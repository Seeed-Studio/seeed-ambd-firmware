/**
*****************************************************************************************
*     Copyright (C) 2020  Seeed Technology Co.,Ltd. All rights reserved.
*****************************************************************************************
   * @file      ble_server.c
   * @brief     
   * @author    Hongtai.Liu
   * @date      2020-08-26
   * @version   v1.0
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2020 Seeed Technology Co.,Ltd</center></h2>
   **************************************************************************************
  */

/** Add Includes here **/
#include <bt_flags.h>
#include <string.h>
#include <os_mem.h>
#include <ble_server.h>

static ble_service_list_t ble_service_list[BLE_SERVER_MAX_APPS];

void write_post_callback(uint8_t conn_id, T_SERVER_ID service_id, uint16_t attrib_index,
                         uint16_t length, uint8_t *p_value)
{
   (void)p_value;
   log_d("write_post_callback: conn_id %d, service_id %d, attrib_index 0x%x, length %d",
         conn_id, service_id, attrib_index, length);
}

/**
 * @brief read characteristic data from service.
 *
 * @param service_id          ServiceID of characteristic data.
 * @param attrib_index        Attribute index of getting characteristic data.
 * @param offset              Used for Blob Read.
 * @param p_length            length of getting characteristic data.
 * @param pp_value            data got from service.
 * @return Profile procedure result
*/
T_APP_RESULT ble_service_attr_read_cb(uint8_t conn_id, T_SERVER_ID service_id,
                                      uint16_t attrib_index, uint16_t offset, uint16_t *p_length, uint8_t **pp_value)
{
   T_APP_RESULT cause = APP_RESULT_SUCCESS;
   log_d("ble_service_attr_read_cb");
   return (cause);
}

/**
 * @brief write characteristic data from service.
 *
 * @param conn_id
 * @param service_id        ServiceID to be written.
 * @param attrib_index      Attribute index of characteristic.
 * @param length            length of value to be written.
 * @param p_value           value to be written.
 * @return Profile procedure result
*/
T_APP_RESULT ble_service_attr_write_cb(uint8_t conn_id, T_SERVER_ID service_id,
                                       uint16_t attrib_index, T_WRITE_TYPE write_type, uint16_t length, uint8_t *p_value,
                                       P_FUN_WRITE_IND_POST_PROC *p_write_ind_post_proc)
{
   log_d("ble_service_attr_write_cb write_type = 0x%x", write_type);
   *p_write_ind_post_proc = write_post_callback;
   T_APP_RESULT cause = APP_RESULT_SUCCESS;
   return cause;
}

/**
 * @brief update CCCD bits from stack.
 *
 * @param conn_id           connection id.
 * @param service_id          Service ID.
 * @param index          Attribute index of characteristic data.
 * @param cccbits         CCCD bits from stack.
 * @return None
*/
void ble_service_cccd_update_cb(uint8_t conn_id, T_SERVER_ID service_id, uint16_t index, uint16_t cccbits)
{
   log_d("ble_service_cccd_update_cb");
   return;
}

/**
 * @brief ble Service Callbacks.
*/
const T_FUN_GATT_SERVICE_CBS ble_service_cbs = {
    ble_service_attr_read_cb,  // Read callback function pointer
    ble_service_attr_write_cb, // Write callback function pointer
    ble_service_cccd_update_cb // CCCD update callback function pointer
};

static void print_ble_desc_list(uint8_t app_id, uint8_t char_handle)
{
   ble_char_list_t *p_char_list_tail = ble_service_list[app_id].char_list;
   bool is_match = false;
   while (p_char_list_tail != NULL)
   {
      if (p_char_list_tail->handle == char_handle)
      {
         is_match = true;
         break;
      }
      p_char_list_tail = p_char_list_tail->next;
   }

   if (is_match == false)
      return;

   ble_desc_list_t *p_desc_list_tail = p_char_list_tail->desc_list;

   printf("\tdesc_list: length: %d \n\r", p_char_list_tail->desc_length);
   while (p_desc_list_tail != NULL)
   {
      printf("\t\t===============================\n\r");
      printf("\t\tdesc handle %d \n\r", p_desc_list_tail->handle);
      printf("\t\tflags: %d\n\r", p_desc_list_tail->desc.flags);
      printf("\t\tUUID: ");
      if (p_desc_list_tail->desc.uuid_length == 16)
      {
         printf("\t%02x%02x%02x%02x:%02x%02x%02x%02x:%02x%02x%02x%02x:%02x%02x%02x%02x\n\r",
                p_desc_list_tail->desc.uuid[0], p_desc_list_tail->desc.uuid[1], p_desc_list_tail->desc.uuid[2], p_desc_list_tail->desc.uuid[3],
                p_desc_list_tail->desc.uuid[4], p_desc_list_tail->desc.uuid[5], p_desc_list_tail->desc.uuid[6], p_desc_list_tail->desc.uuid[7],
                p_desc_list_tail->desc.uuid[8], p_desc_list_tail->desc.uuid[9], p_desc_list_tail->desc.uuid[10], p_desc_list_tail->desc.uuid[11],
                p_desc_list_tail->desc.uuid[12], p_desc_list_tail->desc.uuid[13], p_desc_list_tail->desc.uuid[14], p_desc_list_tail->desc.uuid[15]);
      }
      else
      {
         printf("\t0X%02X%02X\n\r", p_desc_list_tail->desc.uuid[0], p_desc_list_tail->desc.uuid[1]);
      }
      printf("\t\tvalue");
      if (p_desc_list_tail->desc.p_value != NULL)
      {
         printf("(p_value): ");
         for (uint8_t k = 0; k < p_desc_list_tail->desc.vlaue_length; k++)
         {
            printf("\t%02X ", p_desc_list_tail->desc.p_value[k]);
         }
      }
      else
      {
         printf("(uuid): ");
         for (uint8_t k = 0; k < p_desc_list_tail->desc.vlaue_length; k++)
         {
            printf("\t%02X ", p_desc_list_tail->desc.uuid[k + 2]);
         }
      }
      printf("\n\r");
      printf("\t\t===============================\n\r");
      p_desc_list_tail = p_desc_list_tail->next;
   }
}

static void print_ble_char_list(uint8_t app_id)
{
   ble_char_list_t *p_char_list_tail = ble_service_list[app_id].char_list;
   printf("char_list: length: %d\n\r", ble_service_list[app_id].char_length);
   while (p_char_list_tail != NULL)
   {
      printf("\t*******************************\n\r");
      printf("\tCHAR handle %d \n\r", p_char_list_tail->handle);
      printf("\tflags: %d\n\r", p_char_list_tail->CHAR.flags);
      printf("\tproperties: %d\n\r", p_char_list_tail->CHAR.properties);
      printf("\tUUID: ");
      if (p_char_list_tail->CHAR.uuid_length == 16)
      {
         printf("\t%02x%02x%02x%02x:%02x%02x%02x%02x:%02x%02x%02x%02x:%02x%02x%02x%02x\n\r",
                p_char_list_tail->CHAR.uuid[0], p_char_list_tail->CHAR.uuid[1], p_char_list_tail->CHAR.uuid[2], p_char_list_tail->CHAR.uuid[3],
                p_char_list_tail->CHAR.uuid[4], p_char_list_tail->CHAR.uuid[5], p_char_list_tail->CHAR.uuid[6], p_char_list_tail->CHAR.uuid[7],
                p_char_list_tail->CHAR.uuid[8], p_char_list_tail->CHAR.uuid[9], p_char_list_tail->CHAR.uuid[10], p_char_list_tail->CHAR.uuid[11],
                p_char_list_tail->CHAR.uuid[12], p_char_list_tail->CHAR.uuid[13], p_char_list_tail->CHAR.uuid[14], p_char_list_tail->CHAR.uuid[15]);
      }
      else
      {
         printf("\t0X%02X%02X\n\r", p_char_list_tail->CHAR.uuid[0], p_char_list_tail->CHAR.uuid[1]);
      }
      printf("\tvalue");
      if (p_char_list_tail->CHAR.p_value != NULL)
      {
         printf("(p_value): ");
         for (uint8_t k = 0; k < p_char_list_tail->CHAR.vlaue_length; k++)
         {
            printf("\t%02X ", p_char_list_tail->CHAR.p_value[k]);
         }
      }
      else
      {
         printf("(uuid): ");
         for (uint8_t k = 0; k < p_char_list_tail->CHAR.vlaue_length; k++)
         {
            printf("\t%02X ", p_char_list_tail->CHAR.uuid[k + 4]);
         }
      }
      printf("\n\r");
      print_ble_desc_list(app_id, p_char_list_tail->handle);
      printf("\t*******************************\n\r");
      p_char_list_tail = p_char_list_tail->next;
   }
}

void print_ble_serive_list()
{
   printf("print_ble_serive_list: length: %d\n\r", BLE_SERVER_MAX_APPS);
   for (uint8_t i = 0; i < BLE_SERVER_MAX_APPS; i++)
   {
      if (ble_service_list[i].is_alloc)
      {
         printf("-------------------------------\n\r");
         printf("service app_id %d \n\r", i);
         printf("service handle %d \n\r", ble_service_list[i].handle);
         printf("service attr num %d \n\r", ble_service_list[i].attr_num);
         printf("flags: %d\n\r", ble_service_list[i].service.flags);
         if (ble_service_list[i].service.is_primary)
            printf("is primary: true\n\r");
         else
            printf("is primary: false\n\r");

         printf("UUID: ");
         if (ble_service_list[i].service.uuid_length == 16)
         {
            printf("%02x%02x%02x%02x:%02x%02x%02x%02x:%02x%02x%02x%02x:%02x%02x%02x%02x\n\r",
                   ble_service_list[i].service.uuid[0], ble_service_list[i].service.uuid[1], ble_service_list[i].service.uuid[2], ble_service_list[i].service.uuid[3],
                   ble_service_list[i].service.uuid[4], ble_service_list[i].service.uuid[5], ble_service_list[i].service.uuid[6], ble_service_list[i].service.uuid[7],
                   ble_service_list[i].service.uuid[8], ble_service_list[i].service.uuid[9], ble_service_list[i].service.uuid[10], ble_service_list[i].service.uuid[11],
                   ble_service_list[i].service.uuid[12], ble_service_list[i].service.uuid[13], ble_service_list[i].service.uuid[14], ble_service_list[i].service.uuid[15]);
         }
         else
         {
            printf("0X%02X%02X\n\r", ble_service_list[i].service.uuid[0], ble_service_list[i].service.uuid[1]);
         }
         printf("value");
         if (ble_service_list[i].service.p_value != NULL)
         {
            printf("(p_value): ");
            for (uint8_t k = 0; k < ble_service_list[i].service.vlaue_length; k++)
            {
               printf("%02X ", ble_service_list[i].service.p_value[k]);
            }
         }
         else
         {
            printf("(uuid): ");
            for (uint8_t k = 0; k < ble_service_list[i].service.vlaue_length; k++)
            {
               printf("%02X ", ble_service_list[i].service.uuid[k + 4]);
            }
         }
         printf("\n\r");
         printf("characteristic: \n\r");
         print_ble_char_list(i);
         printf("-------------------------------\n\r");
      }
      // else
      // {
      //    printf("Unallocated \n\r");
      // }
   }
}

static uint16_t format_server_attr_tbl(uint8_t app_id, T_ATTRIB_APPL *p_attr_tbl)
{
   if (app_id > BLE_SERVER_MAX_APPS)
      return 0;

   if (ble_service_list[app_id].is_alloc == false)
      return 0;

   if (p_attr_tbl != NULL)
   {
      free(p_attr_tbl);
      p_attr_tbl = NULL;
   }

   T_ATTRIB_APPL *attr_tbl = (T_ATTRIB_APPL *)malloc(sizeof(T_ATTRIB_APPL) * ble_service_list[app_id].attr_num);

   uint16_t attr_index = 0;

   /* attr index 0 Service */
   attr_tbl[attr_index].flags = ble_service_list[app_id].service.flags;
   if (ble_service_list[app_id].service.is_primary)
   {
      attr_tbl[attr_index].type_value[0] = LO_WORD(GATT_UUID_PRIMARY_SERVICE);
      attr_tbl[attr_index].type_value[1] = HI_WORD(GATT_UUID_PRIMARY_SERVICE);
   }
   else
   {
      attr_tbl[attr_index].type_value[0] = LO_WORD(GATT_UUID_SECONDARY_SERVICE);
      attr_tbl[attr_index].type_value[1] = HI_WORD(GATT_UUID_SECONDARY_SERVICE);
   }
   memcpy(&attr_tbl[attr_index].type_value[2], ble_service_list[app_id].service.uuid, 14);
   attr_tbl[attr_index].value_len = ble_service_list[app_id].service.vlaue_length;
   attr_tbl[attr_index].p_value_context = ble_service_list[app_id].service.p_value;
   attr_tbl[attr_index].permissions = ble_service_list[app_id].service.permissions;
   attr_index++;

   ble_char_list_t *p_char_list_tail = ble_service_list[app_id].char_list;

   /* char & desc */
   while (p_char_list_tail != NULL)
   {
      /* char */
      attr_tbl[attr_index].flags = ATTRIB_FLAG_VALUE_INCL;
      attr_tbl[attr_index].type_value[0] = LO_WORD(GATT_UUID_CHARACTERISTIC);
      attr_tbl[attr_index].type_value[1] = HI_WORD(GATT_UUID_CHARACTERISTIC);
      attr_tbl[attr_index].type_value[2] = p_char_list_tail->CHAR.properties;
      attr_tbl[attr_index].value_len = 1;
      attr_tbl[attr_index].p_value_context = NULL;
      attr_tbl[attr_index].permissions = p_char_list_tail->CHAR.permissions;
      attr_index++;
      attr_tbl[attr_index].flags = ATTRIB_FLAG_VALUE_APPL;
      memcpy(attr_tbl[attr_index].type_value, p_char_list_tail->CHAR.uuid, p_char_list_tail->CHAR.uuid_length);
      attr_tbl[attr_index].value_len = 0;
      attr_tbl[attr_index].p_value_context = NULL;
      attr_tbl[attr_index].permissions = p_char_list_tail->CHAR.permissions;
      attr_index++;

      ble_desc_list_t *p_desc_list_tail = p_char_list_tail->desc_list;
      /* desc */
      while (p_desc_list_tail != NULL)
      {
         attr_tbl[attr_index].flags = p_desc_list_tail->desc.flags;
         memcpy(attr_tbl[attr_index].type_value, p_desc_list_tail->desc.uuid, p_desc_list_tail->desc.uuid_length);
         attr_tbl[attr_index].value_len = p_desc_list_tail->desc.vlaue_length;
         attr_tbl[attr_index].p_value_context = p_desc_list_tail->desc.p_value;
         attr_tbl[attr_index].permissions = p_char_list_tail->CHAR.permissions;
         attr_index++;
         p_desc_list_tail = p_desc_list_tail->next;
      }
      p_char_list_tail = p_char_list_tail->next;
   }

   for (int i = 0; i < attr_index; i++)
   {
      printf("----------------------\n\r");
      printf("attr index: %d\n\r", i);
      printf("flags: %d\n\r", attr_tbl[i].flags);
      printf("value: \n\r");
      for (int j = 0; j < 16; j++)
      {
         printf("%02x ", attr_tbl[i].type_value[j]);
      }
      printf("\n\rvalue length: %d\n\r", attr_tbl[i].value_len);
      if (attr_tbl[i].p_value_context != NULL)
      {
         printf("p_value_context: ");
         uint8_t *p_value_context = (uint8_t *)attr_tbl[i].p_value_context;
         for (int j = 0; j < attr_tbl[i].value_len; j++)
         {
            printf("%02x ", p_value_context[j]);
         }
         printf("\n\r");
      }
      else
      {
         printf("p_value_context: NULL\n\r");
      }
      printf("permissions: %d\n\r", (uint8_t *)attr_tbl[i].permissions);
      printf("----------------------\n\r");
   }

   p_attr_tbl = (T_ATTRIB_APPL *)attr_tbl;
   return attr_index;
}

void free_ble_service_list()
{
   for (uint8_t i = 0; i < BLE_SERVER_MAX_APPS; i++)
   {
      if (ble_service_list[i].is_alloc == false)
      {
         continue;
      }
    
      ble_char_list_t *p_char_list_tail = ble_service_list[i].char_list;
      /* char & desc */
      while (p_char_list_tail != NULL)
      {
         ble_desc_list_t *p_desc_list_tail = p_char_list_tail->desc_list;
         while (p_desc_list_tail != NULL)
         {
            ble_desc_list_t *p_desc_list_index = p_desc_list_tail;
            p_desc_list_tail = p_desc_list_tail->next;
            free(p_desc_list_index);
         }
         ble_char_list_t *p_char_list_index = p_char_list_tail;
         p_char_list_tail = p_char_list_tail->next;
         free(p_char_list_index->desc_list);
         free(p_char_list_index);
      }
      free(ble_service_list[i].char_list);
      ble_service_list[i].is_alloc = false;
   }
}

bool ble_server_init(uint8_t num)
{
   log_d("ble_server_init %d\n\r", num);

   server_init(num);

   for (uint8_t i = 0; i < BLE_SERVER_MAX_APPS; i++)
   {
      ble_service_list[i].is_alloc = false;
      ble_service_list[i].handle = 0xff;
      ble_service_list[i].attr_num = 0;
      ble_service_list[i].attr_tbl = NULL;
   }
   return true;
}

/**
  * @brief Add BLE service to the BLE stack database.
  *
  * @param[in]  p_func  Callback when service attribute was read, write or cccd update.
  * @return Service id generated by the BLE stack: @ref T_SERVER_ID.
  * @retval 0xFF Operation failure.
  * @retval others Service id assigned by stack.
  *
  */
T_SERVER_ID ble_add_service(const T_ATTRIB_APPL *ble_service_tbl, uint16_t length)
{
   T_SERVER_ID service_id = 0xff;
   if (false == server_add_service(&service_id,
                                   (uint8_t *)ble_service_tbl,
                                   length,
                                   ble_service_cbs))
   {
      log_e("simp_service_add_service: failure");
      service_id = 0xff;
   }
   else
   {
      log_e("simp_service_add_service: success");
   }

   return service_id;
}

T_SERVER_ID ble_service_start(uint8_t app_id)
{
   if (app_id > BLE_SERVER_MAX_APPS)
      return 0xFF;

   if (ble_service_list[app_id].is_alloc == false)
      return 0xFF;

   T_SERVER_ID service_id = 0xff;

   T_ATTRIB_APPL *attr_tbl = (T_ATTRIB_APPL *)malloc(sizeof(T_ATTRIB_APPL) * ble_service_list[app_id].attr_num);

   uint16_t attr_index = 0;

   /* attr index 0 Service */
   attr_tbl[attr_index].flags = ble_service_list[app_id].service.flags;
   if (ble_service_list[app_id].service.is_primary)
   {
      attr_tbl[attr_index].type_value[0] = LO_WORD(GATT_UUID_PRIMARY_SERVICE);
      attr_tbl[attr_index].type_value[1] = HI_WORD(GATT_UUID_PRIMARY_SERVICE);
   }
   else
   {
      attr_tbl[attr_index].type_value[0] = LO_WORD(GATT_UUID_SECONDARY_SERVICE);
      attr_tbl[attr_index].type_value[1] = HI_WORD(GATT_UUID_SECONDARY_SERVICE);
   }
   memcpy(&attr_tbl[attr_index].type_value[2], ble_service_list[app_id].service.uuid, 14);
   attr_tbl[attr_index].value_len = ble_service_list[app_id].service.vlaue_length;
   attr_tbl[attr_index].p_value_context = ble_service_list[app_id].service.p_value;
   attr_tbl[attr_index].permissions = ble_service_list[app_id].service.permissions;
   attr_index++;

   ble_char_list_t *p_char_list_tail = ble_service_list[app_id].char_list;

   /* char & desc */
   while (p_char_list_tail != NULL)
   {
      /* char */
      attr_tbl[attr_index].flags = ATTRIB_FLAG_VALUE_INCL;
      attr_tbl[attr_index].type_value[0] = LO_WORD(GATT_UUID_CHARACTERISTIC);
      attr_tbl[attr_index].type_value[1] = HI_WORD(GATT_UUID_CHARACTERISTIC);
      attr_tbl[attr_index].type_value[2] = p_char_list_tail->CHAR.properties;
      attr_tbl[attr_index].value_len = 1;
      attr_tbl[attr_index].p_value_context = NULL;
      attr_tbl[attr_index].permissions = p_char_list_tail->CHAR.permissions;
      attr_index++;
      attr_tbl[attr_index].flags = ATTRIB_FLAG_VALUE_APPL;
      memcpy(attr_tbl[attr_index].type_value, p_char_list_tail->CHAR.uuid, p_char_list_tail->CHAR.uuid_length);
      attr_tbl[attr_index].value_len = 0;
      attr_tbl[attr_index].p_value_context = NULL;
      attr_tbl[attr_index].permissions = p_char_list_tail->CHAR.permissions;
      attr_index++;

      ble_desc_list_t *p_desc_list_tail = p_char_list_tail->desc_list;
      /* desc */
      while (p_desc_list_tail != NULL)
      {
         attr_tbl[attr_index].flags = p_desc_list_tail->desc.flags;
         memcpy(attr_tbl[attr_index].type_value, p_desc_list_tail->desc.uuid, p_desc_list_tail->desc.uuid_length);
         attr_tbl[attr_index].value_len = p_desc_list_tail->desc.vlaue_length;
         attr_tbl[attr_index].p_value_context = p_desc_list_tail->desc.p_value;
         attr_tbl[attr_index].permissions = p_desc_list_tail->desc.permissions;
         attr_index++;
         p_desc_list_tail = p_desc_list_tail->next;
      }
      p_char_list_tail = p_char_list_tail->next;
   }

   for (int i = 0; i < attr_index; i++)
   {
      printf("----------------------\n\r");
      printf("attr index: %d\n\r", i);
      printf("flags: %d\n\r", attr_tbl[i].flags);
      printf("value: \n\r");
      for (int j = 0; j < 16; j++)
      {
         printf("%02x ", attr_tbl[i].type_value[j]);
      }
      printf("\n\rvalue length: %d\n\r", attr_tbl[i].value_len);
      if (attr_tbl[i].p_value_context != NULL)
      {
         printf("p_value_context: ");
         uint8_t *p_value_context = (uint8_t *)attr_tbl[i].p_value_context;
         for (int j = 0; j < attr_tbl[i].value_len; j++)
         {
            printf("%02x ", p_value_context[j]);
         }
         printf("\n\r");
      }
      else
      {
         printf("p_value_context: NULL\n\r");
      }
      printf("permissions: %d\n\r", (uint8_t *)attr_tbl[i].permissions);
      printf("----------------------\n\r");
   }

   if (attr_index != 0)
   {
      service_id = ble_add_service(attr_tbl, attr_index * sizeof(T_ATTRIB_APPL));
      if (service_id != 0xff)
      {
         ble_service_list[app_id].handle = service_id;
         ble_service_list[app_id].attr_tbl = attr_tbl;
         return service_id;
      }
   }

   return 0xFF;
}

uint8_t ble_create_service(ble_service_t service)
{
   if (ble_service_list == NULL)
   {
      return 0xFF;
   }

   uint8_t i = 0;
   for (i = 0; i < BLE_SERVER_MAX_APPS; i++)
   {
      if (ble_service_list[i].is_alloc)
      {
         continue;
      }
      else
      {
         memcpy(&(ble_service_list[i].service), &service, sizeof(ble_service_t));
         ble_service_list[i].is_alloc = true;
         ble_service_list[i].char_list = NULL;
         ble_service_list[i].char_length = 0;
         ble_service_list[i].attr_num = 1;
         break;
      }
   }
   if (i != BLE_SERVER_MAX_APPS)
      return i;

   return 0xFF;
}

void free_attr_tbl(T_SERVER_ID srvc_id)
{
   for (int i = 0; i < BLE_SERVER_MAX_APPS; i++)
   {
      if (ble_service_list[i].handle == srvc_id)
      {
         if (ble_service_list[i].attr_tbl != NULL)
         {
            free(ble_service_list[i].attr_tbl);
            ble_service_list[i].attr_tbl = NULL;
            log_d("srvc id:%d app_id: %d, free attr tbl", srvc_id, i);
            return;
         }
      }
   }
}

bool ble_delete_service(uint8_t app_id)
{

   if (app_id > BLE_SERVER_MAX_APPS)
      return false;

   ble_service_list[app_id].is_alloc = false;
   ble_service_list[app_id].handle = 0xff;

   return true;
}

uint8_t ble_get_servie_handle(uint8_t app_id)
{
   if (app_id > BLE_SERVER_MAX_APPS)
      return 0xFF;

   return ble_service_list[app_id].handle;
}

uint8_t ble_create_char(uint8_t app_id, ble_char_t CHAR)
{
   if (app_id > BLE_SERVER_MAX_APPS)
      return 0xFF;

   if (ble_service_list[app_id].is_alloc == false)
      return 0xFF;

   ble_char_list_t *ble_char_list_item = malloc(sizeof(ble_char_list_t));
   memcpy(&(ble_char_list_item->CHAR), &CHAR, sizeof(ble_char_t));
   ble_char_list_item->desc_length = 0;
   ble_char_list_item->desc_list = NULL;
   ble_char_list_item->next = NULL;

   ble_char_list_t *p_char_list_tail = ble_service_list[app_id].char_list;
   if (ble_service_list[app_id].char_list == NULL)
   {
      ble_service_list[app_id].char_list = ble_char_list_item;
   }
   else
   {
      while (p_char_list_tail->next != NULL)
      {
         p_char_list_tail = p_char_list_tail->next;
         //printf("char handle:%d desc_num: %d\n\r", p_char_list_tail->handle, p_char_list_tail->desc_length);
      }
      p_char_list_tail->next = ble_char_list_item;
   }
   ble_service_list[app_id].char_length++;
   ble_char_list_item->handle = ble_service_list[app_id].attr_num + 2;
   ble_service_list[app_id].attr_num += 2;

   return ble_char_list_item->handle;
}

uint8_t ble_create_desc(uint8_t app_id, uint8_t char_handle, ble_desc_t desc)
{
   if (app_id > BLE_SERVER_MAX_APPS)
      return 0xFF;

   if (ble_service_list[app_id].is_alloc == false)
      return 0xFF;

   ble_char_list_t *p_char_list_tail = ble_service_list[app_id].char_list;
   bool is_match = false;
   while (p_char_list_tail != NULL)
   {
      if (p_char_list_tail->handle == char_handle)
      {
         is_match = true;
         break;
      }
      p_char_list_tail = p_char_list_tail->next;
   }

   if (is_match == false)
      return 0xff;

   ble_desc_list_t *ble_desc_list_item = malloc(sizeof(ble_desc_list_t));
   memcpy(&(ble_desc_list_item->desc), &desc, sizeof(ble_desc_t));
   ble_desc_list_item->next = NULL;

   ble_desc_list_t *p_desc_list_tail = p_char_list_tail->desc_list;

   if (p_desc_list_tail == NULL)
   {
      p_char_list_tail->desc_list = ble_desc_list_item;
   }
   else
   {
      while (p_desc_list_tail->next != NULL)
      {
         p_desc_list_tail = p_desc_list_tail->next;
      }
      p_desc_list_tail->next = ble_desc_list_item;
   }

   p_char_list_tail->desc_length++;
   ble_desc_list_item->handle = ble_service_list[app_id].attr_num + 1;
   ble_service_list[app_id].attr_num += 1;

   return ble_desc_list_item->handle;
}
