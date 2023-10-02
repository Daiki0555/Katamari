///////////////////////////////////////
// 構造体。
///////////////////////////////////////

// ピクセルシェーダーへの入力
struct SPSIn
{
    float4 pos : SV_POSITION;           //座標
    float3 normal : NORMAL;             //法線
    float3 tangent : TANGENT;           //接ベクトル
    float3 biNormal : BINORMAL;         //従ベクトル
    float2 uv : TEXCOORD0;              //UV座標
    float3 worldPos : TEXCOORD1;        //ワールド座標
};

cbuffer SkyCubeCb : register(b1)
{
    float luminance;    //明るさ
};

///////////////////////////////////////
// 頂点シェーダーの共通処理をインクルードする。
///////////////////////////////////////
#include "ModelVSCommon.h"

TextureCube<float4> g_skyCubeMap : register(t10);

////////////////////////////////////////////////
// グローバル変数。
////////////////////////////////////////////////
StructuredBuffer<float4x4> g_worldMatrixArray   : register(t10);


////////////////////////////////////////////////
// 関数定義。
////////////////////////////////////////////////
// 法線マップから法線を取得。
float3 GetNormalFromNormalMap(float3 normal, float3 tangent, float3 biNormal, float2 uv)
{
	float3 binSpaceNormal = g_normal.SampleLevel(g_sampler, uv, 0.0f).xyz;
	binSpaceNormal = (binSpaceNormal * 2.0f) - 1.0f;

	float3 newNormal = tangent * binSpaceNormal.x + biNormal * binSpaceNormal.y + normal * binSpaceNormal.z;

	return newNormal;
}

/// <summary>
/// ワールド空間の頂点座標を計算する。
/// </summary>
/// <param name="vertexPos">頂点座標</param>
/// <param name="mWorld">ワールド行列</param>
/// <param name="isUsePreComputedVertexBuffer">事前計算済みの頂点バッファを利用している？</param>
float4 CalcVertexPositionInWorldSpace(float4 vertexPos, float4x4 mWorld, uniform bool isUsePreComputedVertexBuffer)
{
    float4 pos;
    if(isUsePreComputedVertexBuffer){
        pos = vertexPos;
    }else{
        pos = mul(mWorld, vertexPos);  // モデルの頂点をワールド座標系に変換
    }

    return pos;
}

/// <summary>
/// ワールドスペースの法線、接ベクトル、従ベクトルを計算する。
/// </summary>
/// <param name="oNormal">法線の出力先</param>
/// <param name="oTangent">接ベクトルの出力先</param>
/// <param name="oBiNormal">従ベクトルの出力先</param>
/// <param name="mWorld">ワールド行列</param>
/// <param name="iNormal">法線</param>
/// <param name="iTanget">接ベクトル</param>
/// <param name="iBiNormal">従ベクトル</param>
/// <param name="isUsePreComputedVertexBuffer">事前計算済み頂点バッファを利用する？/param>
void CalcVertexNormalTangentBiNormalInWorldSpace( 
    out float3 oNormal, 
    out float3 oTangent, 
    out float3 oBiNormal,
    float4x4 mWorld,
    float3 iNormal,
    float3 iTangent,
    float3 iBiNormal,
    uniform bool isUsePreComputedVertexBuffer
)
{
    if( isUsePreComputedVertexBuffer){
        // 事前計算済み頂点バッファを利用する。
        oNormal = iNormal;
        oTangent = iTangent;
        oBiNormal = iBiNormal;
    }else{
        // 
        float3x3 m3x3 = (float3x3)mWorld;
	    oNormal = normalize(mul(m3x3, iNormal));
	    oTangent = normalize(mul(m3x3, iTangent));
	    oBiNormal = normalize(mul(m3x3, iBiNormal));
    }
}

// モデル用の頂点シェーダーのエントリーポイント
SPSIn VSMainCore(SVSIn vsIn, float4x4 mWorldLocal, uniform bool isUsePreComputedVertexBuffer)
{
    SPSIn psIn;
    //頂点座標をワールド座標系に変換する
    psIn.pos=CalcVertexPositionInWorldSpace(
        vsIn.pos,
        mWorldLocal,
        isUsePreComputedVertexBuffer
    );

    //頂点シェーダーからワールド座標を出力
    psIn.worldPos=(float3)psIn.pos;
    psIn.pos=mul(mView,psIn.pos);//ワールド座標からカメラ座標系に変換
    psIn.pos=mul(mProj,psIn.pos);//カメラ座標からスクリーン座標系に変換

    // ワールド空間の法線、接ベクトル、従ベクトルを計算する。
    CalcVertexNormalTangentBiNormalInWorldSpace(
        psIn.normal,
        psIn.tangent,
        psIn.biNormal,
        mWorldLocal,
        vsIn.normal,
        vsIn.tangent,
        vsIn.biNormal,
        isUsePreComputedVertexBuffer
    );

    psIn.uv=vsIn.uv;

    return psIn;
}


/// <summary>
/// スキンなしメッシュ用の頂点シェーダーのエントリー関数。
/// </summary>
SPSIn VSMain(SVSIn vsIn)
{
	return VSMainCore(vsIn, mWorld, false);
}
/// <summary>
/// スキンなしメッシュ用の頂点シェーダーのエントリー関数(インスタンシング描画用)。
/// </summary>
SPSIn VSMainInstancing(SVSIn vsIn, uint instanceID : SV_InstanceID)
{
	return VSMainCore(vsIn, g_worldMatrixArray[instanceID], false);
}

/// <summary>
/// ピクセルシェーダーのエントリー関数。
/// </summary>
float4 PSMain(SPSIn psIn) : SV_Target0
{
    float4 albedoColor;
    float3 normal=normalize(psIn.normal);
    albedoColor=g_skyCubeMap.Sample(g_sampler,normal*-1.0f)*luminance;
    return albedoColor;
}