#pragma once

#include <algorithm>
//============================================================
//	使用例
//	Timer timer_(limitTime); // 制限時間を設定
//	timer_.update(deltaTime); // 経過時間を追加して更新
//	if (timer_.isTimeout()) { // タイムアウトしたかを確認する
//		// 一定時間ごとの処理
//		…省略
//		timer_.reset(); // タイマーをリセットする
//	}
//============================================================

class Timer
{
public:
	// コンストラクタ
	Timer() {};
	Timer(float limitTime) : time_(0), limitTime_(limitTime) {}

	// 更新
	void update(float deltaTime);

	// リセット
	void reset();

	// タイムアウトか？
	bool isTimeout() const;

	// 現在の経過時間を取得
	float getTime() { return time_; };

	// リミット時間を取得
	float getLimitTime() { return limitTime_; };

	// リミット時間に対する経過時間の割合
	float getRatioTime() { return time_ / limitTime_; };

private:
	float time_;		// 現在の時間
	float limitTime_;	// 制限時間
};