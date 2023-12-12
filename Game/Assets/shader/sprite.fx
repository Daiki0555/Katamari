/*!
 * @brief	�X�v���C�g�p�̃V�F�[�_�[�B
 */

cbuffer cb : register(b0){
	float4x4 mvp;		//���[���h�r���[�v���W�F�N�V�����s��B
	float4 mulColor;	//��Z�J���[�B
};

cbuffer SpriteCB : register(b1)
{
    float Degree;
};

struct VSInput{
	float4 pos : POSITION;
	float2 uv  : TEXCOORD0;
};

struct PSInput{
	float4 pos : SV_POSITION;
	float2 uv  : TEXCOORD0;
};

Texture2D<float4> colorTexture : register(t0);	//�J���[�e�N�X�`���B
sampler Sampler : register(s0);

static const float PI =3.14159f;	//円周率
PSInput VSMain(VSInput In) 
{
	PSInput psIn;
	psIn.pos = mul( mvp, In.pos );
	psIn.uv = In.uv;
	return psIn;
}
float4 PSMainCore( PSInput In )
{
	return colorTexture.Sample(Sampler, In.uv) * mulColor;
}

/// <summary>
/// 通常のエントリー関数。
/// </summary>
float4 PSMain(PSInput In) : SV_Target0
{
    return PSMainCore(In);
}

/// <summary>
/// 円形ゲージ用のエントリー関数。
/// </summary>
float4 PSCircleGauge(PSInput In) : SV_Target0
{
	//中心から上方向のベクトル
	//中心からピクセル座標へのベクトル
	float2 Vector1={0.0f,-1.0f};
	float2 Center={0.5f,0.5f};
	float2 Vector2=In.uv-Center;
	Vector1=normalize(Vector1);
	Vector2=normalize(Vector2);

	//内積を求めて逆コサインを取る
	float Deg=acos(dot(Vector1,Vector2));
	Deg=abs(Deg);

	//設定した角度より小さければ表示、大きければピクセルを破棄する
	if(Vector2.x>Vector1.x){
		Deg=PI+(PI-Deg);
	}
	if(Deg>=Degree){
		//ピクセルを破棄する
		clip(-1);
	}
	return PSMainCore(In);
}