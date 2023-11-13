////////////////////////////////////////////////
// 構造体
////////////////////////////////////////////////
//ディレクションライト用の構造体
struct DirectionLig
{
	float3 ligDirection;		//ライトの方向
	float3 ligColor;			//ライトのカラー
	float3 eyePos;				//視点の位置
	float3 ambient;				//環境光の強さ
};

//ポイントライト用の構造体
struct PointLig
{
	float3 ptPosition;			//ポイントライトの位置
	float3 ptColor;				//ポイントライトのカラー
	float  ptRange;				//ポイントライトの影響範囲
};

//スポットライト用の構造体
struct SpotLig
{
	float3 spPosition;			//スポットライトの位置
	float3 spColor;				//スポットライトのカラー	
	float spRange;				//スポットライトの射出範囲
	float3 spDirection;			//スポットライトの射出方向
	float spAngle;				//スポットライトの射出角度

};

//半球ライト
struct HemiLig
{
	float3 groundColor;			//照り返しのライト
	float3 skyColor;			//天球ライト
	float3 groundNormal;		//地面の法線
};

cbuffer LightCb:register(b1)
{
	DirectionLig dirLig;		//ディレクションライト用の定数バッファー
	PointLig ptLig[1];			//ポイントライトの定数バッファー
	SpotLig spLig[1];			//スポットライトの定数バッファー
	HemiLig hemiLig;			//半球ライトの定数バッファー
	int ptNum;					//ポイントライトの数
	int spNum;
	float4x4 mlvp[NUM_SHADOW_MAP];
};