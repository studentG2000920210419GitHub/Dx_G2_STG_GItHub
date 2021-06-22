//FPS�̃\�[�X�t�@�C��

//�w�b�_�t�@�C���ǂݍ���
#include "FPS.h"

//�O���[�o���ϐ�
FPS fps;	//FPS�̊Ǘ�

//�֐�

/// <summary>
/// FPS�l���v�����A�l���X�V����
/// </summary>
/// <param name=""></param>
VOID FPSUpdate(VOID)
{
	//��ԍŏ��̏���
	if (FALSE == fps.IsInitFlg)
	{
		//����J�n�������}�C�N���b�P�ʂŎ擾
		fps.StartTime = GetNowHiPerformanceCount();	//Windows���N�����Ă���o�߂�������(�}�C�N���b)

		fps.IsInitFlg = TRUE;	//�t���O�𗧂Ă�
	}

	//���݂̎������}�C�N���b�P�ʂŎ擾
	fps.NowTime = GetNowHiPerformanceCount();

	//�O��擾�������Ԃ���̌o�ߎ��Ԃ�b(����)�ɕϊ����Ă���Z�b�g
	fps.DeltaTime = (fps.NowTime - fps.OldTime) / 1000000.0f;

	//����擾�������Ԃ�ۑ�
	fps.OldTime = fps.NowTime;

	//�P�t���[���ځ`FPS�ݒ�l�܂ł́A�J�E���g�A�b�v
	if (fps.Count < fps.SampleRate)
	{
		//�J�E���^�𑝂₷
		fps.Count++;
	}
	else
	{
		//FPS�ݒ�l�̃t���[���ŁA����FPS���v�Z

		//���݂̎�������A0�t���[���ڂ̎��Ԃ������AFPS�̐��l�Ŋ���
		//���݂̕���FPS�l���o��
		fps.DrawValue = 1000000.f / ((fps.NowTime - fps.StartTime) / (float)fps.SampleRate);

		//����J�n�������}�C�N���b�P�ʂŎ擾
		fps.StartTime = GetNowHiPerformanceCount();	//Windows���N�����Ă���o�߂�������(�}�C�N���b)

		//�J�E���^������
		fps.Count = 1;
	}

	return;
}

/// <summary>
/// ����FPS�l��`�悷��
/// </summary>
/// <param name=""></param>
VOID FPSDraw(VOID)
{
	if (GAME_DEBUG == TRUE)
	{
		//�������`��
		DrawFormatString(0, GAME_HEIGHT - 20, GetColor(0, 0, 0), "FPS:%.1f", fps.DrawValue);
	}

	return;
}

/// <summary>
/// FPS�ŏ�����҂�
/// </summary>
/// <param name=""></param>
VOID FPSWait(VOID)
{
	//���݂̎���-�ŏ��̎����ŁA���݂������Ă��鎞�����擾����
	LONGLONG resultTime = fps.NowTime - fps.StartTime;

	//�҂ׂ��~���b���@(1�b/FPS�l * ���݂̃t���[����)����A���݂������Ă��鎞��������
	int waitTime = 1000000.0f / fps.Value * fps.Count - resultTime;

	//�}�C�N���b����~���b�ɕϊ�
	waitTime /= 1000.0f;

	//����������������A������҂�
	if (waitTime > 0)
	{
		WaitTimer(waitTime);	//�����~���b�҂�
	}

	//����������OFF�ɂ��Ă��邩�H
	if (GetWaitVSyncFlag() == FALSE)
	{
		//FPS�ő�l�ł͂Ȃ��Ƃ�
		if (fps.Value < GAME_FPS_MAX)
		{
			//�P�b����FPS�l�����A�҂��Ԃ��������Ƃ��́A������FPS�l���グ�Ă��ǂ�
			//�҂��� 10�~���@<= 1�b/60FPS = 16.6666�~���@�������������ł���
			if (waitTime > 0
				&& waitTime <= 1000.0f / fps.Value)
			{
				//fps.Count++;
				fps.Value++;
			}
			else
			{
				//FPS�l���ǂ����Ă��炸�A�x���Ƃ��́AFPS�l��������
				if (fps.Value > GAME_FPS_MIN)
				{
					fps.Value--;
				}
			}
		}
	}

	return;
}