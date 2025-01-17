/*********************************************************************************************************************
* TC264 Opensourec Library ����TC264 ��Դ�⣩��һ�����ڹٷ� SDK �ӿڵĵ�������Դ��
* Copyright (c) 2022 SEEKFREE ��ɿƼ�
*
* ���ļ��� TC264 ��Դ���һ����
*
* TC264 ��Դ�� ���������
* �����Ը���������������ᷢ���� GPL��GNU General Public License���� GNUͨ�ù�������֤��������
* �� GPL �ĵ�3�棨�� GPL3.0������ѡ��ģ��κκ����İ汾�����·�����/���޸���
*
* ����Դ��ķ�����ϣ�����ܷ������ã�����δ�������κεı�֤
* ����û�������������Ի��ʺ��ض���;�ı�֤
* ����ϸ����μ� GPL
*
* ��Ӧ�����յ�����Դ���ͬʱ�յ�һ�� GPL �ĸ���
* ���û�У������<https://www.gnu.org/licenses/>
*
* ����ע����
* ����Դ��ʹ�� GPL3.0 ��Դ����֤Э�� ������������Ϊ���İ汾
* ��������Ӣ�İ��� libraries/doc �ļ����µ� GPL3_permission_statement.txt �ļ���
* ����֤������ libraries �ļ����� �����ļ����µ� LICENSE �ļ�
* ��ӭ��λʹ�ò����������� ���޸�����ʱ���뱣����ɿƼ��İ�Ȩ����������������
*
* �ļ�����          zf_common_font
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          ADS v1.9.20
* ����ƽ̨          TC264D
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����              ����                ��ע
* 2022-08-10        Teternal            first version
* 2023-12-06        Teternal            ���²����߼� �޸������ݶ�ȡʱ�쳣�Ĳ���
********************************************************************************************************************/

#include "zf_common_debug.h"
#include "zf_common_fifo.h"

//-------------------------------------------------------------------------------------------------------------------
// �������     FIFO ͷָ��λ��
// ����˵��     *fifo               FIFO ����ָ��
// ����˵��     offset              ƫ����
// ���ز���     void
// ʹ��ʾ��     fifo_head_offset(fifo, 1);
// ��ע��Ϣ     ���������ļ��ڲ����� �û����ù�ע Ҳ�����޸�
//-------------------------------------------------------------------------------------------------------------------
static void fifo_head_offset (fifo_struct *fifo, uint32 offset)
{
    fifo->head += offset;
    
    while(fifo->max <= fifo->head)                                              // �����Χ���������������С ֱ��С����󻺳�����С
    {
        fifo->head -= fifo->max;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// �������     FIFO βָ��λ��
// ����˵��     *fifo               FIFO ����ָ��
// ����˵��     offset              ƫ����
// ���ز���     void
// ʹ��ʾ��     fifo_end_offset(fifo, 1);
// ��ע��Ϣ     ���������ļ��ڲ����� �û����ù�ע Ҳ�����޸�
//-------------------------------------------------------------------------------------------------------------------
static void fifo_end_offset (fifo_struct *fifo, uint32 offset)
{
    fifo->end += offset;
    
    while(fifo->max <= fifo->end)                                               // �����Χ���������������С ֱ��С����󻺳�����С
    {
        fifo->end -= fifo->max;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// �������     FIFO ���û�����
// ����˵��     *fifo               FIFO ����ָ��
// ���ز���     void
// ʹ��ʾ��     fifo_clear(fifo);
// ��ע��Ϣ     ��յ�ǰ FIFO ������ڴ�
//-------------------------------------------------------------------------------------------------------------------
fifo_state_enum fifo_clear (fifo_struct *fifo)
{
    zf_assert(NULL != fifo);
    fifo_state_enum return_state = FIFO_SUCCESS;                                // ���������ֵ
    do
    {
//        if(FIFO_IDLE != fifo->execution)                                        // �ж��Ƿ�ǰ FIFO �Ƿ����
//        {
//            return_state = FIFO_RESET_UNDO;                                     // ���ò���δ���
//            break;
//        }
        fifo->execution |= FIFO_RESET;                                          // ���ò�����λ
        fifo->head      = 0;                                                    // ���� FIFO ������ֵ��λ
        fifo->end       = 0;                                                    // ���� FIFO ������ֵ��λ
        fifo->size      = fifo->max;                                            // ���� FIFO ������ֵ��λ
        switch(fifo->type)
        {
            case FIFO_DATA_8BIT:    memset(fifo->buffer, 0, fifo->max);     break;
            case FIFO_DATA_16BIT:   memset(fifo->buffer, 0, fifo->max * 2); break;
            case FIFO_DATA_32BIT:   memset(fifo->buffer, 0, fifo->max * 4); break;
        }
        fifo->execution = FIFO_IDLE;                                            // ����״̬��λ
    }while(0);
    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     FIFO ��ѯ��ǰ���ݸ���
// ����˵��     *fifo               FIFO ����ָ��
// ���ز���     uint32              ��ʹ�ó���
// ʹ��ʾ��     uint32 len = fifo_used(fifo);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
uint32 fifo_used (fifo_struct *fifo)
{
    zf_assert(fifo != NULL);
    return (fifo->max - fifo->size);                                            // ���ص�ǰ FIFO �����������ݸ���
}

//-------------------------------------------------------------------------------------------------------------------
// �������     �� FIFO ��д������
// ����˵��     *fifo               FIFO ����ָ��
// ����˵��     dat                 ����
// ���ز���     fifo_state_enum     ����״̬
// ʹ��ʾ��     zf_log(fifo_write_element(&fifo, data) == FIFO_SUCCESS, "fifo_write_byte error");
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
fifo_state_enum fifo_write_element (fifo_struct *fifo, uint32 dat)
{
    zf_assert(NULL != fifo);
    fifo_state_enum return_state = FIFO_SUCCESS;                                // ���������ֵ

    do
    {
        if((FIFO_RESET | FIFO_WRITE) & fifo->execution)                         // ����д��������״̬ ����д�뾺����ָ�����
        {
            return_state = FIFO_WRITE_UNDO;                                     // д�����δ���
            break;
        }
        fifo->execution |= FIFO_WRITE;                                          // д�������λ

        if(1 <= fifo->size)                                                     // ʣ��ռ��㹻װ�±�������
        {
            switch(fifo->type)
            {
                case FIFO_DATA_8BIT:    ((uint8 *)fifo->buffer)[fifo->head]  = (uint8)dat;  break;
                case FIFO_DATA_16BIT:   ((uint16 *)fifo->buffer)[fifo->head] = (uint16)dat; break;
                case FIFO_DATA_32BIT:   ((uint32 *)fifo->buffer)[fifo->head] = dat; break;
            }
            fifo_head_offset(fifo, 1);                                          // ͷָ��ƫ��
            fifo->size -= 1;                                                    // ������ʣ�೤�ȼ�С
        }
        else
        {
            return_state = FIFO_SPACE_NO_ENOUGH;                                // ��ǰ FIFO �������� ������д������ ���ؿռ䲻��
        }
        fifo->execution &= ~FIFO_WRITE;                                         // д�������λ
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     �� FIFO ��д������
// ����˵��     *fifo               FIFO ����ָ��
// ����˵��     *dat                ������Դ������ָ��
// ����˵��     length              ��Ҫд������ݳ���
// ���ز���     fifo_state_enum     ����״̬
// ʹ��ʾ��     zf_log(fifo_write_buffer(&fifo, data, 32) == FIFO_SUCCESS, "fifo_write_buffer error");
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
fifo_state_enum fifo_write_buffer (fifo_struct *fifo, void *dat, uint32 length)
{
    zf_assert(NULL != fifo);
    fifo_state_enum return_state = FIFO_SUCCESS;                                // ���������ֵ
    uint32 temp_length = 0;
    
    do
    {
        if(NULL == dat)
        {
            return_state = FIFO_BUFFER_NULL;                                    // �û��������쳣
            break;
        }
        if((FIFO_RESET | FIFO_WRITE) & fifo->execution)                         // ����д��������״̬ ����д�뾺����ָ�����
        {
            return_state = FIFO_WRITE_UNDO;                                     // д�����δ���
            break;
        }
        fifo->execution |= FIFO_WRITE;                                          // д�������λ

        if(length <= fifo->size)                                                // ʣ��ռ��㹻װ�±�������
        {
            temp_length = fifo->max - fifo->head;                               // ����ͷָ����뻺����β���ж��ٿռ�

            if(length > temp_length)                                            // ���뻺����β���Ȳ���д������ ���λ������ֶβ���
            {
                switch(fifo->type)
                {
                    case FIFO_DATA_8BIT:
                    {
                        memcpy(
                            &(((uint8 *)fifo->buffer)[fifo->head]),
                            dat, temp_length);                                  // ������һ������
                        fifo_head_offset(fifo, temp_length);                    // ͷָ��ƫ��
                        memcpy(
                            &(((uint8 *)fifo->buffer)[fifo->head]),
                            &(((uint8 *)dat)[temp_length]),
                            length - temp_length);                              // �����ڶ�������
                        fifo_head_offset(fifo, length - temp_length);           // ͷָ��ƫ��
                    }break;
                    case FIFO_DATA_16BIT:
                    {
                        memcpy(
                            &(((uint16 *)fifo->buffer)[fifo->head]),
                            dat, temp_length * 2);                              // ������һ������
                        fifo_head_offset(fifo, temp_length);                    // ͷָ��ƫ��
                        memcpy(
                            &(((uint16 *)fifo->buffer)[fifo->head]),
                            &(((uint16 *)dat)[temp_length]),
                            (length - temp_length) * 2);                        // �����ڶ�������
                        fifo_head_offset(fifo, length - temp_length);           // ͷָ��ƫ��
                    }break;
                    case FIFO_DATA_32BIT:
                    {
                        memcpy(
                            &(((uint32 *)fifo->buffer)[fifo->head]),
                            dat, temp_length * 4);                              // ������һ������
                        fifo_head_offset(fifo, temp_length);                    // ͷָ��ƫ��
                        memcpy(
                            &(((uint32 *)fifo->buffer)[fifo->head]),
                            &(((uint32 *)dat)[temp_length]),
                            (length - temp_length) * 4);                        // �����ڶ�������
                        fifo_head_offset(fifo, length - temp_length);           // ͷָ��ƫ��
                    }break;
                }
            }
            else
            {
                switch(fifo->type)
                {
                    case FIFO_DATA_8BIT:
                    {
                        memcpy(
                            &(((uint8 *)fifo->buffer)[fifo->head]),
                            dat, length);                                       // һ������д��
                        fifo_head_offset(fifo, length);                         // ͷָ��ƫ��
                    }break;
                    case FIFO_DATA_16BIT:
                    {
                        memcpy(
                            &(((uint16 *)fifo->buffer)[fifo->head]),
                            dat, length * 2);                                   // һ������д��
                        fifo_head_offset(fifo, length);                         // ͷָ��ƫ��
                    }break;
                    case FIFO_DATA_32BIT:
                    {
                        memcpy(
                            &(((uint32 *)fifo->buffer)[fifo->head]),
                            dat, length * 4);                                   // һ������д��
                        fifo_head_offset(fifo, length);                         // ͷָ��ƫ��
                    }break;
                }
            }

            fifo->size -= length;                                               // ������ʣ�೤�ȼ�С
        }
        else
        {
            return_state = FIFO_SPACE_NO_ENOUGH;                                // ��ǰ FIFO �������� ������д������ ���ؿռ䲻��
        }
        fifo->execution &= ~FIFO_WRITE;                                         // д�������λ
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     �� FIFO ��ȡ����
// ����˵��     *fifo               FIFO ����ָ��
// ����˵��     *dat                Ŀ�껺����ָ��
// ����˵��     flag                �Ƿ��� FIFO ״̬ ��ѡ���Ƿ���ն�ȡ������
// ���ز���     fifo_state_enum     ����״̬
// ʹ��ʾ��     zf_log(fifo_read_element(&fifo, data, FIFO_READ_ONLY) == FIFO_SUCCESS, "fifo_read_byte error");
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
fifo_state_enum fifo_read_element (fifo_struct *fifo, void *dat, fifo_operation_enum flag)
{
    zf_assert(NULL != fifo);
    fifo_state_enum return_state = FIFO_SUCCESS;                                // ���������ֵ

    do
    {
        if(NULL == dat)
        {
            return_state = FIFO_BUFFER_NULL;                                    // �û��������쳣
        }
        else
        {
            if((FIFO_RESET | FIFO_CLEAR) & fifo->execution)                     // �ж��Ƿ�ǰ FIFO �Ƿ���ִ����ջ����ò���
            {
                return_state = FIFO_READ_UNDO;                                  // ��ȡ����δ���
                break;
            }

            if(1 > fifo_used(fifo))
            {
                return_state = FIFO_DATA_NO_ENOUGH;                             // ������û������ �������ݳ��Ȳ���
                break;                                                          // ֱ���˳�����
            }

            fifo->execution |= FIFO_READ;                                       // ��������λ
            switch(fifo->type)
            {
                case FIFO_DATA_8BIT:    *((uint8 *)dat) = ((uint8 *)fifo->buffer)[fifo->end];   break;
                case FIFO_DATA_16BIT:   *((uint16 *)dat) = ((uint16 *)fifo->buffer)[fifo->end]; break;
                case FIFO_DATA_32BIT:   *((uint32 *)dat) = ((uint32 *)fifo->buffer)[fifo->end]; break;
            }
            fifo->execution &= ~FIFO_READ;                                      // ��������λ
        }

        if(FIFO_READ_AND_CLEAN == flag)                                         // ���ѡ���ȡ������ FIFO ״̬
        {
            if((FIFO_RESET | FIFO_CLEAR | FIFO_READ) == fifo->execution)        // ���� ���� ��� ��ȡ ״̬ �����쳣
            {
                return_state = FIFO_CLEAR_UNDO;                                 // ��ղ���δ���
                break;
            }
            fifo->execution |= FIFO_CLEAR;                                      // �������λ
            fifo_end_offset(fifo, 1);                                           // �ƶ� FIFO ͷָ��
            fifo->size += 1;                                                    // �ͷŶ�Ӧ���ȿռ�
            fifo->execution &= ~FIFO_CLEAR;                                     // �������λ
        }
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     �� FIFO ��ȡ����
// ����˵��     *fifo               FIFO ����ָ��
// ����˵��     *dat                Ŀ�껺����ָ��
// ����˵��     *length             ��ȡ�����ݳ��� ���û����ô����������ᱻ�޸�
// ����˵��     flag                �Ƿ��� FIFO ״̬ ��ѡ���Ƿ���ն�ȡ������
// ���ز���     fifo_state_enum     ����״̬
// ʹ��ʾ��     zf_log(fifo_read_buffer(&fifo, data, &length, FIFO_READ_ONLY) == FIFO_SUCCESS, "fifo_read_buffer error");
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
fifo_state_enum fifo_read_buffer (fifo_struct *fifo, void *dat, uint32 *length, fifo_operation_enum flag)
{
    zf_assert(NULL != fifo);
    zf_assert(NULL != length);
    fifo_state_enum return_state = FIFO_SUCCESS;                                // ���������ֵ
    uint32 temp_length = 0;
    uint32 fifo_data_length = 0;

    do
    {
        if(NULL == dat)
        {
            return_state = FIFO_BUFFER_NULL;
        }
        else
        {
            if((FIFO_RESET | FIFO_CLEAR) & fifo->execution)                     // �ж��Ƿ�ǰ FIFO �Ƿ���ִ����ջ����ò���
            {
                *length = fifo_data_length;                                     // ������ȡ�ĳ���
                return_state = FIFO_READ_UNDO;                                  // ��ȡ����δ���
                break;
            }

            fifo_data_length = fifo_used(fifo);                                 // ��ȡ��ǰ�����ж���
            if(*length > fifo_data_length)                                      // �жϳ����Ƿ��㹻
            {
                *length = fifo_data_length;                                     // ������ȡ�ĳ���
                return_state = FIFO_DATA_NO_ENOUGH;                             // ��־���ݲ���
                if(0 == fifo_data_length)                                       // ���û������ ��ֱ���˳�
                {
                    fifo->execution &= ~FIFO_READ;                              // ��������λ
                    break;
                }
            }

            fifo->execution |= FIFO_READ;                                       // ��������λ
            temp_length = fifo->max - fifo->end;                                // ����βָ����뻺����β���ж��ٿռ�
            if(*length <= temp_length)                                          // �㹻һ���Զ�ȡ���
            {
                switch(fifo->type)
                {
                    case FIFO_DATA_8BIT:    memcpy(dat, &(((uint8 *)fifo->buffer)[fifo->end]), *length);        break;
                    case FIFO_DATA_16BIT:   memcpy(dat, &(((uint16 *)fifo->buffer)[fifo->end]), *length * 2);   break;
                    case FIFO_DATA_32BIT:   memcpy(dat, &(((uint32 *)fifo->buffer)[fifo->end]), *length * 4);   break;
                }
            }
            else
            {
                switch(fifo->type)
                {
                    case FIFO_DATA_8BIT:
                    {
                        memcpy(dat, &(((uint8 *)fifo->buffer)[fifo->end]), temp_length);
                        memcpy(&(((uint8 *)dat)[temp_length]), fifo->buffer, *length - temp_length);
                    }break;
                    case FIFO_DATA_16BIT:
                    {
                        memcpy(dat, &(((uint16 *)fifo->buffer)[fifo->end]), temp_length * 2);
                        memcpy(&(((uint16 *)dat)[temp_length]), fifo->buffer, (*length - temp_length) * 2);
                    }break;
                    case FIFO_DATA_32BIT:
                    {
                        memcpy(dat, &(((uint32 *)fifo->buffer)[fifo->end]), temp_length * 4);
                        memcpy(&(((uint32 *)dat)[temp_length]), fifo->buffer, (*length - temp_length) * 4);
                    }break;
                }
            }
            fifo->execution &= ~FIFO_READ;                                      // ��������λ
        }

        if(FIFO_READ_AND_CLEAN == flag)                                         // ���ѡ���ȡ������ FIFO ״̬
        {
            if((FIFO_RESET | FIFO_CLEAR | FIFO_READ) == fifo->execution)        // ���� ���� ��� ��ȡ ״̬ �����쳣
            {
                return_state = FIFO_CLEAR_UNDO;                                 // ��ղ���δ���
                break;
            }
            fifo->execution |= FIFO_CLEAR;                                      // �������λ
            fifo_end_offset(fifo, *length);                                     // �ƶ� FIFO ͷָ��
            fifo->size += *length;                                              // �ͷŶ�Ӧ���ȿռ�
            fifo->execution &= ~FIFO_CLEAR;                                     // �������λ
        }
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     �� FIFO β����ȡָ������ buffer
// ����˵��     *fifo               FIFO ����ָ��
// ����˵��     *dat                Ŀ�껺����ָ��
// ����˵��     *length             ��ȡ�����ݳ��� ���û����ô����������ᱻ�޸�
// ����˵��     flag                �Ƿ��� FIFO ״̬ ��ѡ���Ƿ���ն�ȡ������
// ���ز���     fifo_state_enum     ����״̬
// ʹ��ʾ��     zf_log(fifo_read_tail_buffer(&fifo, data, &length, FIFO_READ_ONLY) == FIFO_SUCCESS, "fifo_read_buffer error");
// ��ע��Ϣ     ���ʹ�� FIFO_READ_AND_CLEAN ���� ���ᶪ���������ݲ�������� FIFO
//              ���ʹ�� FIFO_READ_AND_CLEAN ���� ���ᶪ���������ݲ�������� FIFO
//              ���ʹ�� FIFO_READ_AND_CLEAN ���� ���ᶪ���������ݲ�������� FIFO
//-------------------------------------------------------------------------------------------------------------------
fifo_state_enum fifo_read_tail_buffer (fifo_struct *fifo, void *dat, uint32 *length, fifo_operation_enum flag)
{
    zf_assert(NULL != fifo);
    zf_assert(NULL != length);
    fifo_state_enum return_state = FIFO_SUCCESS;                                // ���������ֵ
    uint32 temp_length = 0;
    uint32 fifo_data_length = 0;

    do
    {
        if(NULL == dat)
        {
            return_state = FIFO_BUFFER_NULL;
        }
        else
        {
            if((FIFO_RESET | FIFO_CLEAR | FIFO_WRITE) & fifo->execution)        // �ж��Ƿ�ǰ FIFO �Ƿ���ִ����ջ����ò���
            {
                *length = fifo_data_length;                                     // ������ȡ�ĳ���
                return_state = FIFO_READ_UNDO;                                  // ��ȡ����δ���
                break;
            }

            fifo_data_length = fifo_used(fifo);                                 // ��ȡ��ǰ�����ж���
            if(*length > fifo_data_length)                                      // �жϳ����Ƿ��㹻
            {
                *length = fifo_data_length;                                     // ������ȡ�ĳ���
                return_state = FIFO_DATA_NO_ENOUGH;                             // ��־���ݲ���
                if(0 == fifo_data_length)                                       // ���û������ ��ֱ���˳�
                {
                    fifo->execution &= ~FIFO_READ;                              // ��������λ
                    break;
                }
            }

            fifo->execution |= FIFO_READ;                                       // ��������λ
            if((fifo->head > fifo->end) || (fifo->head >= *length))
            {
                switch(fifo->type)
                {
                    case FIFO_DATA_8BIT:    memcpy(dat, &(((uint8 *)fifo->buffer)[fifo->head - *length]), *length);     break;
                    case FIFO_DATA_16BIT:   memcpy(dat, &(((uint16 *)fifo->buffer)[fifo->head - *length]), *length * 2);break;
                    case FIFO_DATA_32BIT:   memcpy(dat, &(((uint32 *)fifo->buffer)[fifo->head - *length]), *length * 4);break;
                }
            }
            else
            {
                temp_length = *length - fifo->head;                             // ����βָ����뻺����β���ж��ٿռ�
                switch(fifo->type)
                {
                    case FIFO_DATA_8BIT:
                    {
                        memcpy(dat, &(((uint8 *)fifo->buffer)[fifo->max - temp_length]), temp_length);
                        memcpy(&(((uint8 *)dat)[temp_length]), &(((uint8 *)fifo->buffer)[fifo->head - *length]), (*length - temp_length));
                    }break;
                    case FIFO_DATA_16BIT:
                    {
                        memcpy(dat, &(((uint16 *)fifo->buffer)[fifo->max - temp_length]), temp_length * 2);
                        memcpy(&(((uint16 *)dat)[temp_length]), &(((uint16 *)fifo->buffer)[fifo->head - *length]), (*length - temp_length) * 2);
                    }break;
                    case FIFO_DATA_32BIT:
                    {
                        memcpy(dat, &(((uint32 *)fifo->buffer)[fifo->max - temp_length]), temp_length * 4);
                        memcpy(&(((uint32 *)dat)[temp_length]), &(((uint32 *)fifo->buffer)[fifo->head - *length]), (*length - temp_length) * 4);
                    }break;
                }
            }
            fifo->execution &= ~FIFO_READ;                                      // ��������λ
        }

        if(FIFO_READ_AND_CLEAN == flag)                                         // ���ѡ���ȡ������ FIFO ״̬
        {
            if((FIFO_RESET | FIFO_CLEAR | FIFO_READ) == fifo->execution)        // ���� ���� ��� ��ȡ ״̬ �����쳣
            {
                return_state = FIFO_CLEAR_UNDO;                                 // ��ղ���δ���
                break;
            }
            fifo_clear(fifo);
        }
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     FIFO ��ʼ�� ���ض�Ӧ������
// ����˵��     *fifo               FIFO ����ָ��
// ����˵��     type                FIFO ����λ��
// ����˵��     *buffer_addr        Ҫ���صĻ�����
// ����˵��     size                ��������С
// ���ز���     fifo_state_enum     ����״̬
// ʹ��ʾ��     fifo_init(&user_fifo, user_buffer, 64);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
fifo_state_enum fifo_init (fifo_struct *fifo, fifo_data_type_enum type, void *buffer_addr, uint32 size)
{
    zf_assert(NULL != fifo);
    fifo_state_enum return_state = FIFO_SUCCESS;
    do
    {
        fifo->buffer    = buffer_addr;
        fifo->execution = FIFO_IDLE;
        fifo->type      = type;
        fifo->head      = 0;
        fifo->end       = 0;
        fifo->size      = size;
        fifo->max       = size;
    }while(0);
    return return_state;
}