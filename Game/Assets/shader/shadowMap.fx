/*!
 * @brief シャドウマップ描画用のシェーダー
 */

//モデル用の定数バッファー
cbuffer ModelCb:register(b0)
{
    float4x4 mWorld;
    float4x4 mView;
    float4x4 mProj;
};

struct SSkinVSIn{
	int4  Indices  	: BLENDINDICES0;
    float4 Weights  : BLENDWEIGHT0;
};

//頂点シェーダーへの入力
struct SVSIn
{
    float4 pos : POSITION; //モデルの頂点座標
    float2 uv : TEXCOORD0; //UV座標
    SSkinVSIn skinVert;    //スキン用のデータ
};

//ピクセルシェーダーへの入力
struct SPSIn
{
    float4 pos : SV_POSITION; //スクリーン空間でのピクセルの座標
    float2 uv : TEXCOORD0;    //UV座標
};

///////////////////////////////////////////////////
// グローバル変数
///////////////////////////////////////////////////
StructuredBuffer<float4x4> g_boneMatrix : register(t3);	//ボーン行列
StructuredBuffer<float4x4> g_worldMatrixArray   : register(t10);	//ワールド行列の配列。インスタンシング描画の際に有効。

/// <summary>
//スキン行列を計算する。
/// </summary>
float4x4 CalcSkinMatrix(SSkinVSIn skinVert)
{
    float4x4 skinning=0;
    float w=0.0f;
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
/// 頂点シェーダー
/// <summary>
SPSIn VSMainCore(SVSIn vsIn,float4x4 mWorldLocal)
{
    SPSIn psIn;
    float4x4 m;
	m = mWorldLocal;
    psIn.pos = mul(m, vsIn.pos);
    psIn.pos = mul(mView, psIn.pos);
    psIn.pos = mul(mProj, psIn.pos);
    psIn.uv = vsIn.uv;
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


/// <summary>
/// シャドウマップ描画用のピクセルシェーダー
/// </summary>
float4 PSMain(SPSIn psIn) : SV_Target0
{
    //シャドウマップ描画用のピクセルシェーダーを作成する
    return float4 (psIn.pos.z,psIn.pos.z,psIn.pos.z,1.0f);
}