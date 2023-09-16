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
// グローバル変数。
////////////////////////////////////////////////
Texture2D<float4> g_albedo : register(t0);				//アルベドマップ
Texture2D<float4> g_normalMap : register(t1);			//法線
Texture2D<float4> g_worldPos : register(t2);			//ワールド座標

////////////////////////////////////////////////
// 定数バッファ。
////////////////////////////////////////////////
//モデル用の定数バッファ
cbuffer cb : register(b0)
{
	float4x4 mvp;
	float4 mulColor;
};

///////////////////////////////////////
// サンプラステート。
///////////////////////////////////////
sampler g_sampler : register(s0);

////////////////////////////////////////////////
// 関数定義。
////////////////////////////////////////////////
float3 CalcNormal(SPSIn psIn);

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
	float3 s_worldPos=g_worldPos.Sample(g_sampler,psIn.uv).xyz;

    return albedo;
}