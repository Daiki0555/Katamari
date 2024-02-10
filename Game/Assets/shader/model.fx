/*!
 * @brief	シンプルなモデルシェーダー。
 */

///////////////////////////////////////
// 頂点シェーダーの共通処理をインクルードする。
///////////////////////////////////////
#include "ModelVSCommon.h"

///////////////////////////////////////
// ライト情報
///////////////////////////////////////
#include "LightCB.h"

////////////////////////////////////////////////
// グローバル変数。
////////////////////////////////////////////////
Texture2D<float4> g_depthTexture : register(t10);

StructuredBuffer<float4x4> g_worldMatrixArray   : register(t11);	//ワールド行列の配列。インスタンシング描画の際に有効。

////////////////////////////////////////////////
// 定数バッファ。
////////////////////////////////////////////////

//ピクセルシェーダーへの入力。
struct SPSIn{
	float4 pos : SV_POSITION; //座標。
	float3 normal : NORMAL; //法線。
	float3 tangent : TANGENT; //接ベクトル。
	float3 biNormal : BINORMAL; //従ベクトル。
	float2 uv : TEXCOORD0; //UV座標。
	float3 worldPos : TEXCOORD1; // ワールド座標
	float4 posInLVP : TEXCOORD3;	//ライトビュースクリーン空間でのピクセルの座標
	float4 posInProj    : TEXCOORD4;
};

////////////////////////////////////////////////
// 関数定義。
////////////////////////////////////////////////
float4 CalcOutline(SPSIn psIn);
float3 CalcLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal);
float3 CalcPhongSpecular(float3 lightDirection, float3 lightColor, float3 worldPos, float3 normal,float specMap);
float3 CalcLigFromPointLight(SPSIn psIn, float3 normal,float specMap);
float CalcLim(float3 dirDirection, float3 normal,float3 worldPos);
float3 CalcNormal(SPSIn psIn);
float3 CalcSpecular(float2 uv);

/// <summary>
//スキン行列を計算する。
/// </summary>
float4x4 CalcSkinMatrix(SSkinVSIn skinVert)
{
	float4x4 skinning = 0;	
	float w = 0.0f;
	[unroll]
    for (int i = 0; i < 3; i++)
    {
        skinning += g_boneMatrix[skinVert.Indices[i]] * skinVert.Weights[i];
        w += skinVert.Weights[i];
    }
    
    skinning += g_boneMatrix[skinVert.Indices[3]] * (1.0f - w);
	
    return skinning;
}


/// <summary>
/// 頂点シェーダーのコア関数。
/// </summary>
SPSIn VSMainCore(SVSIn vsIn, float4x4 mWorldLocal)
{
	SPSIn psIn;
	float4x4 m;
	m = mWorldLocal;
    float4 worldPos = mul(m, vsIn.pos);
    psIn.pos=mul(m,vsIn.pos);      //モデルの頂点をワールド座標系に変換
    psIn.worldPos=worldPos;             //頂点シェーダーからワールド座標を出力
    psIn.pos=mul(mView,psIn.pos);       //ワールド座標系からカメラ座標系に変換
    psIn.pos=mul(mProj,psIn.pos);       //カメラ座標系からスクリーン座標系に変換
    psIn.normal=normalize(mul(m,vsIn.normal));

    //接ベクトルと従ベクトルをワールド空間に変換する
    psIn.tangent=normalize(mul(m,vsIn.tangent));
    psIn.biNormal=normalize(mul(m,vsIn.biNormal));

	//頂点の正規化スクリーン座標系の座標をピクセルシェーダーに渡す
    psIn.posInProj = psIn.pos;
    psIn.posInProj.xy /= psIn.posInProj.w;
    psIn.uv=vsIn.uv;

	return psIn;
}

/// <summary>
/// スキンなしメッシュ用の頂点シェーダーのエントリー関数。
/// </summary>
SPSIn VSMain(SVSIn vsIn)
{
	return VSMainCore(vsIn, mWorld);
}
/// <summary>
/// スキンありメッシュの頂点シェーダーのエントリー関数。
/// </summary>
SPSIn VSSkinMain( SVSIn vsIn ) 
{
	return VSMainCore(vsIn, CalcSkinMatrix(vsIn.skinVert));
}

/// <summary>
/// スキンなしメッシュ用の頂点シェーダーのエントリー関数(インスタンシング描画用)。
/// </summary>
SPSIn VSMainInstancing(SVSIn vsIn,uint instanceID : SV_InstanceID)
{
	return VSMainCore(vsIn,g_worldMatrixArray[instanceID]);
}

float4 CalcOutline(SPSIn psIn)
{
	float4 albedoColor = g_albedo.Sample(g_sampler, psIn.uv);
	//近傍8テクセルの深度値を計算して、エッジを抽出する
	// 正規化スクリーン座標系からUV座標系に変換する
    float2 uv = psIn.posInProj.xy * float2( 0.5f, -0.5f) + 0.5f;

    // 近傍8テクセルへのUVオフセット
    float2 uvOffset[8] = {
        float2(           0.0f,  1.0f / 720.0f), //上
        float2(           0.0f, -1.0f / 720.0f), //下
        float2( 1.0f / 1280.0f,           0.0f), //右
        float2(-1.0f / 1280.0f,           0.0f), //左
        float2( 1.0f / 1280.0f,  1.0f / 720.0f), //右上
        float2(-1.0f / 1280.0f,  1.0f / 720.0f), //左上
        float2( 1.0f / 1280.0f, -1.0f / 720.0f), //右下
        float2(-1.0f / 1280.0f, -1.0f / 720.0f)  //左下
    };

    // このピクセルの深度値を取得
    float depth = g_depthTexture.Sample(g_sampler, uv).x;

	
    // 近傍8テクセルの深度値の平均値を計算する
    float depth2 = 0.0f;
    for( int i = 0; i < 8; i++)
    {
        depth2 += g_depthTexture.Sample(g_sampler, uv + uvOffset[i]).x;
    }
    depth2 /= 8.0f;

    // 自身の深度値と近傍8テクセルの深度値の差を調べる
    if(abs(depth - depth2) > 0.00005f)
    {
        // 深度値が結構違う場合はピクセルカラーを黒にする
        // ->これがエッジカラーとなる
        return float4( 0.0f, 0.0f, 0.0f, 1.0f);
    }
	return albedoColor;
}

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
float3 CalcPhongSpecular(float3 lightDirection, float3 lightColor, float3 worldPos, float3 normal,float specMap)
{
	 
	//反射ベクトルを求める
	float3 refVec=reflect(lightDirection,normal);

	//光があたったサーフェイスから視点に伸びるベクトルを求める
	float3 toEye=dirLig.eyePos-worldPos;

	//正規化する
	toEye=normalize(toEye);

	//鏡面反射光の強さを求める
	float t=dot(refVec,toEye);

	t=max(t,0.0f);

	//鏡面反射の強さを求める
	t=pow(t,2.0f);

	//鏡面反射光を求める
	return lightColor*t*specMap;

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
/// ポイントライトによる反射光を計算
/// </summary>
/// <param name="psIn">ピクセルシェーダーに渡されている引数</param>
float3 CalcLigFromPointLight(SPSIn psIn, float3 normal,float specMap)
{
	
	float3 finalPtLig = (0.0f, 0.0f, 0.0f);

	for(int i=0;i<ptNum;i++)
	{
		//ポイントライトによるLambert拡散反射光とPhong鏡面反射光を計算する
		//サーフェイスに入射するポイントライトの光の向きを計算する
		
		float3 ligDir=psIn.worldPos-ptLig[i].ptPosition;
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
			psIn.worldPos,
			normal,
			specMap
		);
		
		//ポイントライトとの距離を計算する
		float distance=length(psIn.worldPos-ptLig[i].ptPosition);

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
/// 法線を計算する
/// </summary>
float3 CalcNormal(SPSIn psIn)
{
	float3 normal =psIn.normal;
	//法線マップからタンジェントスペースの法線をサンプリングする
	float3 localNormal=g_normalMap.Sample(g_sampler,psIn.uv).xyz;
	//タンジェントスペースの法線を0～１の範囲から-1～１の範囲に復元する
	localNormal=(localNormal-0.5f)*2.0f;

	normal=psIn.tangent*localNormal.x
				+ psIn.biNormal*localNormal.y
				+normal*localNormal.z;
	return normal;

}

/// <summary>
/// スペキュラを計算
/// </summary>
float3 CalcSpecular(float2 uv)
{
		//スペキュラマップからスペキュラ反射をサンプリング
	float specPower=g_worldPos.Sample(g_sampler,uv).r;

	return specPower*10.0f;
}

/// <summary>
/// ピクセルシェーダーのエントリー関数。
/// </summary>
float4 PSMainCore( SPSIn psIn ,uniform bool isShadow) : SV_Target0
{
	
	float4 albedoColor=CalcOutline(psIn);

	//法線によるライティングを計算する
	float3 normal=CalcNormal(psIn);

	//スペキュラマップを計算する
	float specMap=CalcSpecular(psIn.uv);
	//ディレクションライトによるLambert拡散反射光を計算する
	float3 diffDirection=CalcLambertDiffuse(dirLig.ligDirection,dirLig.ligColor,normal);

	// ディレクションライトによるPhong鏡面反射光を計算する
	float3 specuDirection=CalcPhongSpecular(dirLig.ligDirection,dirLig.ligColor,psIn.worldPos,normal,specMap);

	//リムライトによるライティングを計算する
	float limLight=CalcLim(dirLig.ligDirection,normal,psIn.worldPos);

	//ポイントライトによるライティングを計算する
	float3 pointLight=CalcLigFromPointLight(psIn,normal,specMap); 

	//最終的な反射光にリムライトの反射光を合算する
	float3 limColor=limLight*dirLig.ligColor;

	float3 lig=0.0f;

	lig += diffDirection
		+ specuDirection
		+ dirLig.ambient
		+ limColor
		+ pointLight;
		;
	albedoColor.xyz*=lig;

	//法線とライトの方向とで内積を計算する
	float t =dot(psIn.normal,-dirLig.ligDirection);

	//内積の結果が 0.1 以下なら、最終カラーの色味を若干落とす
	if(t<0.1f){
		albedoColor.xyz*=0.8f;
	}

	return albedoColor;
}

/// <summary>
/// シャドウ無しのエントリー関数。
/// </summary>
float4 PSMain( SPSIn psIn ) : SV_Target0
{
	return PSMainCore(psIn, false);
}

/// <summary>
/// シャドウ有りのエントリー関数。
/// </summary>
float4 PSMainShadow( SPSIn psIn ) : SV_Target0
{
	return PSMainCore(psIn, true);
}
