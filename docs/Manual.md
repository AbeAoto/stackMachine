# �}�j���A��

## ���߃Z�b�g�ꗗ

| �I�y�R�[�h | �I�y�����h |                      |                       | �Ӗ�                                                                                      |
| ---------- | ---------- | -------------------- | --------------------- | ----------------------------------------------------------------------------------------- |
| PUSH       | int num    |                      |                       | �X�^�b�N�̈�ԏ�ɒl num ��ς�                                                           |
| POP        |            |                      |                       | �X�^�b�N�̈�ԏ�̗v�f���폜����                                                          |
| SETLOCAL   | string srt | int num              |                       | �ϐ��� str �ɒl num ��ۑ�����                                                            |
| GETLOCAL   | string str |                      |                       | �ϐ��� str �̒l���X�^�b�N�̈�ԏ�ɐς�                                                   |
| ALLOCARR   | string str | unsigned int num     |                       | �T�C�Y num �̔z�� str �̗̈���m�ۂ���                                                    |
| SETARR     | string str | int idx (string var) | int num (string var2) | �z�� str �̃C���f�b�N�X idx(var) �Ԗڂɒl num(var2) ��ۑ�����                            |
| GETARR     | string str | int idx (string var) |                       | �z�� str �̃C���f�b�N�X idx(var) �Ԗڂ̒l���X�^�b�N�̈�ԏ�ɐς�                         |
| FREEARR    | string str |                      |                       | �z�� str �̗̈���������                                                                 |
| FUNC       |            |                      |                       |                                                                                           |
| CALL       |            |                      |                       |                                                                                           |
| RET        |            |                      |                       |                                                                                           |
| ADD        |            |                      |                       | �X�^�b�N�̏��̗v�f�𑫂��ăX�^�b�N�̈�ԏ�ɐς�                                      |
| SUB        |            |                      |                       | �X�^�b�N�̏ォ��(��Ԗ�) - (��Ԗ�)�̒l���X�^�b�N�̈�ԏ�ɐς�                           |
| MUL        |            |                      |                       | �X�^�b�N�̏��̗v�f����Z���ăX�^�b�N�̈�ԏ�ɐς�                                    |
| DIV        |            |                      |                       | �X�^�b�N�̏ォ��(��Ԗڂ�) / (��Ԗ�) �����ăX�^�b�N�̈�ԏ�ɐς�                        |
| PRINT      |            |                      |                       | �X�^�b�N�̈�ԏ�̒l��W���o�͂���                                                        |
| JUMP       | string str |                      |                       | ���x�� str �ɃW�����v����                                                                 |
| JPEQ0      | string str |                      |                       | �X�^�b�N�̈�ԏオ0�ł������烉�x�� str �ɃW�����v����                                    |
| GT         |            |                      |                       | �X�^�b�N�̏ォ�� (��Ԗ�) > (��Ԗ�) �Ȃ� 1 ���A�����łȂ��Ȃ� 0 ���X�^�b�N�̈�ԏ�ɐς� |
| LT         |            |                      |                       | �X�^�b�N�̏ォ�� (��Ԗ�) < (��Ԗ�) �Ȃ� 1 ���A�����łȂ��Ȃ� 0 ���X�^�b�N�̈�ԏ�ɐς� |
| LOGNOT     |            |                      |                       | �X�^�b�N�̈�ԏ�̘_���ے�l���A�X�^�b�N�̈�ԏ�ɐς�                                    |
| END        |            |                      |                       | �v���O�����̏I����錾����                                                                |
