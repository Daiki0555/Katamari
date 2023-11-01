#pragma once
class MoveLR :public IMove
{
public:
	~MoveLR()override;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="rot"></param>
	void Init(const Vector3& pos, const Quaternion& rot) override;


	/// <summary>
	/// 移動処理
	/// </summary>
	const Vector3& Move() override;
public:
private:
private:
};
