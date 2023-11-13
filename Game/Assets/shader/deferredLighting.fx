///////////////////////////////////////
// 定数
///////////////////////////////////////
static const float PI = 3.1415926f;         // π
static const int NUM_SHADOW_MAP = 3;        // シャドウマップの枚数。

///////////////////////////////////////
// PBRライティング
///////////////////////////////////////
#include "PBRLighting.h"

///////////////////////////////////////
// ライト情報
///////////////////////////////////////
#include "LightCB.h"

//頂点シェーダーへの入力。
struct SVSIn{
	float4 pos 		: POSITION;		//モデルの頂点座標。
	float2 uv 		: TEXCOORD0;	//UV座標。
};

//ピクセルシェーダーへの入力。
struct SPSIn{
	float4 pos 		: SV_POSITION;	//スクリーン空間でのピクセルの座標。
	float2 uv 		: TEXCOORD0;	//uv座標。
};

////////////////////////////////////////////////
// 定数バッファ。
////////////////////////////////////////////////
//モデル用の定数バッファ
cbuffer cb : register(b0)
{
	float4x4 mvp;
	float4 mulColor;
};

////////////////////////////////////////////////
// グローバル変数。
////////////////////////////////////////////////
Texture2D<float4> g_albedo : register(t0);				//アルベドマップ
Texture2D<float4> g_normalMap : register(t1);			//法線
Texture2D<float4> g_worldPos : register(t2);			//ワールド座標
Texture2D<float4> g_metallicShadowSmoothTexture : register(t3); // メタリック、シャドウ、スムーステクスチャ。rに金属度、gに影パラメータ、aに滑らかさ。
Texture2D<float4> g_shadowMap[NUM_SHADOW_MAP] : register(t4);  //シャドウマップ。
///////////////////////////////////////
// サンプラステート。
///////////////////////////////////////
sampler g_sampler : register(s0);

////////////////////////////////////////////////
// 関数定義。
////////////////////////////////////////////////
float3 CalcLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal);
float3 CalcPhongSpecular(float3 lightDirection, float3 lightColor, float3 worldPos, float3 normal);
float3 CalcLigFromPointLight(SPSIn psIn, float3 normal,float3 worldPos);
float3 CalcLigFromSpotLight(SPSIn psIn, float3 normal,float3 worldPos);
float CalcLim(float3 dirDirection, float3 normal,float3 worldPos);
float3 CalcLigFromHemiSphereLight(float3 normal,float3 groundColor, float3 skyColor, float3 groundNormal);
float3 CalcNormal(SPSIn psIn);
float3 CalcSpecular(float3 normal,SPSIn psIn);
float CalcShadowMap(float3 worldPos,SPSIn psIn);

/// <summary>
/// Lambert拡散反射光を計算する
/// </summary>
float3 CalcLambertDiffuse(float3 lightDirection,float3 lightColor,float3 normal)
{
	//ピクセルの法線とライト方向の内積を計算する
	float t=dot(normal,lightDirection)*-1.0f;

	//内積の値を0以上の値にする
	t=max(t,0.0f);

	//拡散反射光を計算する
	return lightColor*t;
} 

/// <summary>
/// Phong鏡面反射光を計算する
/// </summary>
float3 CalcPhongSpecular(float3 lightDirection, float3 lightColor, float3 worldPos, float3 normal)
{
	 
	//反射ベクトルを求める
	float3 refVec=reflect(lightDirection,normal);

	//光があたったサーフェイスから視点に伸びるベクトルを求める
	// 視線に向かって伸びるベクトルを計算する
	float3 toEye=normalize(dirLig.eyePos-worldPos);

	//鏡面反射光の強さを求める
	float t=dot(refVec,toEye);

	t=max(t,0.0f);

	//鏡面反射の強さを求める
	t=pow(t,2.0f);

	//鏡面反射光を求める
	return lightColor*t;

}

/// <summary>
/// ポイントライトによる反射光を計算
/// </summary>
/// <param name="psIn">ピクセルシェーダーに渡されている引数</param>
float3 CalcLigFromPointLight(SPSIn psIn, float3 normal,float3 worldPos)
{
	
	float3 finalPtLig = (0.0f, 0.0f, 0.0f);

	for(int i=0;i<ptNum;i++)
	{
		//ポイントライトによるLambert拡散反射光とPhong鏡面反射光を計算する
		//サーフェイスに入射するポイントライトの光の向きを計算する
		
		float3 ligDir=worldPos-ptLig[i].ptPosition;
		ligDir = normalize(ligDir);
		//減衰なしのLambert拡散反射光を計算する
		float3 diffPoint=CalcLambertDiffuse(
			ligDir,
			ptLig[i].ptColor,
			normal
		);

		//減衰なしのPhong鏡面反射光を計算する
		float3 specPoint=CalcPhongSpecular(
			ligDir,
			ptLig[i].ptColor,
			worldPos,
			normal
		);
		
		//ポイントライトとの距離を計算する
		float distance=length(worldPos-ptLig[i].ptPosition);

		//影響率は距離に比例して小さくなっていく
		float affect=1.0f-1.0f/ptLig[i].ptRange*distance;
		affect=max(affect,0.0f);

		//影響率を指数関数的にする
		affect=pow(affect,3.0f);

		//拡散反射光と鏡面反射光に減衰率を乗算して影響率を弱める
		diffPoint*=affect;
		specPoint*=affect;
	 	finalPtLig += diffPoint + specPoint;
	}
	return finalPtLig;
}

/// <summary>
/// スポットライトを計算する
/// </summary>
float3 CalcLigFromSpotLight(SPSIn psIn, float3 normal,float3 worldPos)
{
	float3 finalspLig = (0.0f, 0.0f, 0.0f);
	//スポットライトによるライティングを計算する
	for(int i=0;i<spNum;i++)
	{
		//ピクセル座標ースポットライトの座標を計算
		float3 ligDir=worldPos-spLig[i].spPosition;

		//正規化して大きさ1のベクトルにする
		ligDir=normalize(ligDir);

		//減衰なしのLambert拡散反射光を計算する
		float3 diffSpotLight=CalcLambertDiffuse(
			ligDir,				//ライトの方向
			spLig[i].spColor,	//ライトのカラー
			normal				//サーフェイスの法線
		);

		//減衰なしのPhong鏡面反射を計算する
		float3 specSpotLight=CalcPhongSpecular(
			ligDir,				//ライトの方向	
			spLig[i].spColor,	//ライトのカラー
			worldPos,		//サーフェイスのワールド座標
			normal				//サーフェイスの法線
		);
		//距離による影響率を計算する
    	// スポットライトとの距離を計算する
		float3 distance =length(worldPos-spLig[i].spPosition);

		//影響率は距離に比例して小さくなっていく
		float affect =1.0f-1.0f/spLig[i].spRange*distance;
		affect=max(affect,0.0f);

		//影響率を指数関数的にする
		affect=pow(affect,3.0f);

		//影響率を乗算して反射光を弱める
		diffSpotLight*=affect;
		specSpotLight*=affect;

		//入射光と射出方向の角度を求める
    	//dot()を利用して内積を求める
		float angle=dot(ligDir,spLig[i].spDirection);

		//dot()で求めた値をacos()に渡して角度を求める
		angle=abs(acos(angle));

		 // 角度による影響率を求める
    	// 角度に比例して小さくなっていく影響率を計算する
    	affect = 1.0f - 1.0f / spLig[i].spAngle * angle;
		affect=max(affect,0.0f);

		// 影響の仕方を指数関数的にする。
    	affect = pow(affect, 0.5f);

		diffSpotLight*=affect;
		specSpotLight*=affect;

		finalspLig=diffSpotLight+specSpotLight;
	}
	return finalspLig;
}

/// <summary>
/// リムライトを計算する
/// </summary>
float CalcLim(float3 dirDirection, float3 normal,float3 worldPos)
{
	//光があたったサーフェイスから視点に伸びるベクトルを求める
	// 視線に向かって伸びるベクトルを計算する
	float3 toEye=normalize(dirLig.eyePos-worldPos);

	//サーフェイスの法線と光の入射方向に依存するリムの強さを求める
	float power1=1.0f-max(0.0f,dot(dirDirection,normal));

	//サーフェイスの法線と視線の方向に依存するリムの強さを求める
	float power2=1.0f-max(0.0f,dot(-toEye,normal));

	//最終的なリムの強さを求める
	float limPower=power1*power2;

	//pow()関数を使用して、強さの変化を指数関数的にする
	limPower=pow(limPower,3.0f);


	return limPower;
}

/// <summary>
/// 半球ライトを計算する
/// </summary>
float3 CalcLigFromHemiSphereLight(float3 normal,float3 groundColor, float3 skyColor, float3 groundNormal)
{
	//半球ライトを計算する
	//サーフェイスの法線と地面の法線との内積を計算する
	float t=dot(normal,groundNormal);

	//内積の結果を0～１の範囲に変換する
	t=(t+1.0f)/2.0f;

	//地面色と天球色を補完率tで線形補間する
	float3 hemiLight=lerp(groundColor,skyColor,t);

	return hemiLight;

}

/// <summary>
/// 法線を計算する
/// </summary>
float3 CalcNormal(SPSIn psIn)
{
	//法線マップからタンジェントスペースの法線をサンプリングする
	float3 localNormal=g_normalMap.Sample(g_sampler,psIn.uv).xyz;
	//タンジェントスペースの法線を0～１の範囲から-1～１の範囲に復元する
	localNormal=(localNormal-0.5f)*2.0f;

	return localNormal;

}

/// <summary>
/// スペキュラを計算
/// </summary>
float3 CalcSpecular(float3 normal,SPSIn psIn)
{
		//スペキュラマップからスペキュラ反射をサンプリング
	float3 worldPos=g_worldPos.Sample(g_sampler,psIn.uv).xyz;
	float3 toEye=normalize(dirLig.eyePos-worldPos);
	float3 r=reflect(dirLig.ligDirection,normal);
	float t=max(0.0f,dot(toEye,r));
	t=pow(t,5.0f);

	return t;
}

float CalcShadowMap(float3 worldPos,SPSIn psIn)
{
	float shadow=0.0f;
	for(int cascadeIndex = 0; cascadeIndex < NUM_SHADOW_MAP; cascadeIndex++){
		float4 posInLVP=mul(mlvp[cascadeIndex],float4(worldPos,1.0f));
		float2 shadowMapUV=posInLVP.xy/posInLVP.w;
		float zInLVP=posInLVP.z/posInLVP.w;
		shadowMapUV*=float2(0.5f,-0.5f);
		shadowMapUV+=0.5f;
		// シャドウマップUVが範囲内か判定
		if(shadowMapUV.x>=0.0f&&shadowMapUV.x<=1.0f
		&&shadowMapUV.y>=0.0f&&shadowMapUV.y<=1.0f){
			// シャドウマップから値をサンプリング
		    float2 shadowValue=g_shadowMap[cascadeIndex].Sample(g_sampler, shadowMapUV).xy;
			  // まずこのピクセルが遮蔽されているか調べる
            if(zInLVP >= shadowValue.r + 0.001f)
            {
                shadow = 0.5f;
            }
            break;
        }
    }
    return shadow;

}



/// <summary>
/// 頂点シェーダーのコア関数。
/// </summary>
SPSIn VSMain(SVSIn vsIn)
{
	SPSIn psIn;
	psIn.pos = mul(mvp, vsIn.pos);
    psIn.uv = vsIn.uv;
    return psIn;
}

/// <summary>
/// ピクセルシェーダーのエントリー関数。
/// </summary>
float4 PSMain( SPSIn psIn) : SV_Target0
{
    //アルベドマップを読み込む
    float4 albedo = g_albedo.Sample(g_sampler, psIn.uv);

	//法線によるライティングを計算する
	float3 normalMap=CalcNormal(psIn);

    //ワールド座標をサンプリング
	float3 worldPos=g_worldPos.Sample(g_sampler,psIn.uv).xyz;

	//スペキュラカラーはアルベドカラーと同じにする
	float3 specColor=albedo;

	// 金属度と滑らかさをサンプリング
    float metallic = g_metallicShadowSmoothTexture.SampleLevel(g_sampler, psIn.uv,0).r;
    
	 //スムース
    float smooth = g_metallicShadowSmoothTexture.SampleLevel(g_sampler, psIn.uv, 0).a;

	//影生成用のパラメータ。
    float shadowParam = g_metallicShadowSmoothTexture.Sample(g_sampler, psIn.uv).g;
	
	float3 lig=0.0f;

	//影の落ち具合を計算する
	float shadow=CalcShadowMap(worldPos,psIn)*shadowParam;

	//視線に向かって伸びるベクトルを計算する
	float3 toEye=normalize(dirLig.eyePos-worldPos);
	
	if(shadow<0.5)
	{
		// ディズニーベースの拡散反射を実装する
        // フレネル反射を考慮した拡散反射を計算
		float diffuseFromFresnel=CalcDiffuseFromFresnel(normalMap,-dirLig.ligDirection,toEye);

		//正規化Lambert拡散反射を求める
		float NdotL=saturate(dot(normalMap,-dirLig.ligDirection));
		float3 lambertDiffuse=dirLig.ligColor*NdotL/PI;

		//最終的な拡散反射光
		float3 diffuse=albedo*diffuseFromFresnel*lambertDiffuse;

		
        // クックトランスモデルを利用した鏡面反射率を計算する
        // クックトランスモデルの鏡面反射率を計算する
		float3 spec=CookTorranceSpecular(-dirLig.ligDirection,toEye,normalMap,smooth)*dirLig.ligColor;

		// 金属度が高ければ、鏡面反射はスペキュラカラー、低ければ白
        // スペキュラカラーの強さを鏡面反射率として扱う
		spec*=lerp(float3(1.0f,1.0f,1.0f),specColor,metallic);

		// 滑らかさを使って、拡散反射光と鏡面反射光を合成する
        // 滑らかさが高ければ、拡散反射は弱くなる
		lig+=diffuse*(1.0f-smooth)+spec*smooth;;
		
		//ポイントライトによるライティングを計算する
		float3 pointLight=CalcLigFromPointLight(psIn,normalMap,worldPos); 
		//スポットライトによるライティングを計算する
		float3 spotLight=CalcLigFromSpotLight(psIn,normalMap,worldPos);
    	//リムライトによるライティングを計算する
		float limLight=CalcLim(dirLig.ligDirection,normalMap,worldPos);
		//半球ライトによるライティングを計算する
		float3 hemiSphereLight=CalcLigFromHemiSphereLight(normalMap,hemiLig.groundColor, hemiLig.skyColor, hemiLig.groundNormal);

	
		//最終的な反射光にリムライトの反射光を合算する
		float3 limColor=limLight*dirLig.ligColor;
		//すべてを足して最終的な光を求める
		lig = dirLig.ambient
		 +pointLight
		 +spotLight
		 +limColor
		 +hemiSphereLight
		 ;

	}
	
	lig=min(lig,10.0f);
	
	albedo.xyz *= lig;

    return albedo;
}