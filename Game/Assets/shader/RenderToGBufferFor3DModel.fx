//モデル用の定数バッファー
cbuffer ModelCb:register(b0)
{
    float4x4 mWorld;
    float4x4 mView;
    float4x4 mProj;
};

//スキニング用の頂点データをひとまとめ。
struct SSkinVSIn{
	int4  Indices  	: BLENDINDICES0;
    float4 Weights  : BLENDWEIGHT0;
};

//頂点シェーダーへの入力
struct SVSIn
{
    float4 pos : POSITION;      //座標
    float3 normal : NORMAL;     //法線
    float3 tangent : TANGENT;  //接ベクトル
    float3 biNormal : BINORMAL; //縦ベクトル     
    float2 uv :TEXCOORD0;
    SSkinVSIn skinVert;				//スキン用のデータ。
};

// ピクセルシェーダーへの入力
struct SPSIn
{
    float4 pos : SV_POSITION;       //座標。
    float3 normal : NORMAL;         //法線。
    float3 tangent  : TANGENT;      //接ベクトル。
    float3 biNormal : BINORMAL;     //従ベクトル。
    float2 uv : TEXCOORD0;          //UV座標。
    float3 worldPos : TEXCOORD1;    // ワールド座標
};


//ピクセルシェーダーからの出力
struct SPSOut
{
    float4 albedo : SV_Target0; //アルベド
    float4 normal : SV_Target1; //法線
    float4 worldPos :SV_Target2; //ワールド座標
    float4 metaricShadowSmooth : SV_Target3;  // メタリック、影パラメータ、スムース。rにメタリック、gに影パラメータ、aにスムース。
};

//モデルのテクスチャ
Texture2D<float4>g_texture : register(t0);
Texture2D<float4> g_normal : register(t1);      //法線マップ
Texture2D<float4> g_metalSmooth : register(t2);     //金属度と滑らかさマップ
StructuredBuffer<float4x4> g_boneMatrix : register(t3);	//ボーン行列。

//サンプラーステート
sampler g_sampler :register(s0);

// 法線マップから法線を取得。
float3 GetNormalFromNormalMap(float3 normal,float3 tangent,float3 biNormal,float2 uv)
{
    float3 binSpaceNormal=g_normal.SampleLevel(g_sampler,uv,0.0f).xyz;
    binSpaceNormal=(binSpaceNormal*2.0f)-1.0f;
    float3 newNormal = tangent * binSpaceNormal.x + biNormal * binSpaceNormal.y + normal * binSpaceNormal.z;
    return newNormal;
}

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
/// モデル用の頂点シェーダーのエントリーポイント
/// </summary>
SPSIn VSMainCore(SVSIn vsIn,uniform bool hasSkin)
{
    SPSIn psIn;
	float4x4 m;
	if( hasSkin ){
		m = CalcSkinMatrix(vsIn.skinVert);
	}else{
		m = mWorld;
	}
    float4 worldPos = mul(m, vsIn.pos);
    psIn.pos=mul(m,vsIn.pos);      //モデルの頂点をワールド座標系に変換
    psIn.worldPos=worldPos;             //頂点シェーダーからワールド座標を出力
    psIn.pos=mul(mView,psIn.pos);       //ワールド座標系からカメラ座標系に変換
    psIn.pos=mul(mProj,psIn.pos);       //カメラ座標系からスクリーン座標系に変換
    psIn.normal=normalize(mul(m,vsIn.normal));

    //接ベクトルと従ベクトルをワールド空間に変換する
    psIn.tangent=normalize(mul(m,vsIn.tangent));
    psIn.biNormal=normalize(mul(m,vsIn.biNormal));

    psIn.uv=vsIn.uv;

    return psIn;
}

/// <summary>
/// スキンなしメッシュ用の頂点シェーダーのエントリー関数。
/// </summary>
SPSIn VSMain(SVSIn vsIn)
{
	return VSMainCore(vsIn, false);
}
/// <summary>
/// スキンありメッシュの頂点シェーダーのエントリー関数。
/// </summary>
SPSIn VSSkinMain( SVSIn vsIn ) 
{
	return VSMainCore(vsIn, true);
}

SPSOut PSMainCore(SPSIn psIn,int isShadowReciever)
{
    //G-Bufferに出力
    SPSOut psOut;

    //アルベドカラーを出力
    psOut.albedo=g_texture.Sample(g_sampler,psIn.uv);

    clip(psOut.albedo.a - 0.2f);    // ピクセルキル

    psOut.albedo.w = psIn.pos.z;
    
    //法線を出力
    psOut.normal.xyz=GetNormalFromNormalMap(
        psIn.normal,psIn.tangent,psIn.biNormal,psIn.uv);
    psOut.normal.w=1.0f;
    // 符号なしバッファに描きこむので、法線の範囲を-1～1を0～1に変換する。
    psOut.normal = ( psOut.normal * 0.5f ) + 0.5f;

    
    //ワールド座標を出力
    psOut.worldPos=float4(psIn.worldPos,1.0f);

   // メタリックスムースを出力。
    psOut.metaricShadowSmooth = g_metalSmooth.Sample(g_sampler, psIn.uv);
    // 影パラメータ。
    psOut.metaricShadowSmooth.g = 255.0f * isShadowReciever;
    return psOut;
}

/// <summary>
/// シャドウ無しのエントリー関数。
/// </summary>
SPSOut PSMain( SPSIn psIn ) : SV_Target0
{
	return PSMainCore(psIn, 0);
}

/// <summary>
/// シャドウ有りのエントリー関数。
/// </summary>
SPSOut PSMainShadow( SPSIn psIn ) : SV_Target0
{
	return PSMainCore(psIn, 1);
}
