// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/OVGameState.h"

AOVGameState::AOVGameState()
{
	BossHp = 100.0f;
	BossStart = false;
	BossDead = false;
}

void AOVGameState::SetBossHp(float NewHp)
{
	BossHp = NewHp;
	OnBossHpChanged.Broadcast(BossHp);
}



float AOVGameState::GetBossHp() const
{
	return BossHp;
}

void AOVGameState::SetCharacterDead(bool NewDead)
{
	CharacterDead = NewDead;
}

bool AOVGameState::GetCharacterDead() const
{
	return CharacterDead;
}

void AOVGameState::BossState(bool start)
{
	BossStart = start;
	OnBossAttackState.Broadcast(BossStart);
}
