///////////////////////////////////////
// 定数
///////////////////////////////////////
static const int NUM_SHADOW_MAP = 3;        // シャドウマップの枚数。
///////////////////////////////////////
// 定数バッファ。
///////////////////////////////////////
// モデル用の定数バッファー
cbuffer ModelCb : register(b0)
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


////////////////////////////////////////////////
// 構造体
////////////////////////////////////////////////

// 頂点シェーダーへの入力
struct SVSIn
{
    float4 pos : POSITION;          //頂点座標。
    float3 normal : NORMAL;         //法線。
    float2 uv : TEXCOORD0;          //UV座標。
    float3 tangent  : TANGENT;      //接ベクトル。
    float3 biNormal : BINORMAL;     //従ベクトル。
    SSkinVSIn skinVert;				//スキン用のデータ。
};

////////////////////////////////////////////////
// グローバル変数。
////////////////////////////////////////////////
Texture2D<float4> g_albedo : register(t0);				//アルベドマップ
Texture2D<float4> g_normalMap : register(t1);			//法線
Texture2D<float4> g_worldPos : register(t2);			//スペキュラ
Texture2D<float4> g_shadowMap[NUM_SHADOW_MAP] : register(t4);

StructuredBuffer<float4x4> g_boneMatrix : register (t3);	//ボーン行列。

///////////////////////////////////////
// サンプラーステート
///////////////////////////////////////
sampler g_sampler : register(s0);


////////////////////////////////////////////////
// 関数定義。
////////////////////////////////////////////////


