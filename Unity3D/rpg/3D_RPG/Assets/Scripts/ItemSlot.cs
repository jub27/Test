using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;
using System.Text;
using System;
using System.Net.NetworkInformation;

public class ItemSlot : MonoBehaviour, IPointerClickHandler
{
    public int item_id;
    public int item_nums;

    public bool equiped;
    public bool shop_item;
    public bool quick;
    public int price;

    public Image image_child;
    private Image image;
    public Text itemNums_Text;
    public GameObject itemInfo;
    public int index;
    private PlayerStatus ps;
    
    private void Awake()
    {
        image = GetComponent<Image>();
        itemNums_Text = GetComponentInChildren<Text>();
        ps = GameObject.Find("Player").GetComponent<PlayerStatus>();
        item_nums = 0;
        equiped = false;
        shop_item = false;
        quick = false;
        price = 0;
    }

    public void SetItem(int id)
    {
        item_id = id;
        image_child.sprite = ItemSystem.instance.item_Sprites[item_id];
        image_child.color = new Color(1, 1, 1, 1);
        item_nums = 1;
        if (item_id < 1000)
        {
            image.color = InventorySystem.instance.outLine[(int)ItemSystem.instance.consume_dict[item_id].itemGrade];
            price = ItemSystem.instance.consume_dict[item_id].price;
            itemInfo.GetComponentInChildren<Text>().text = ItemSystem.instance.consume_dict[item_id].itemInfo +"\n판매가격 : " + price.ToString();
        }
        else if(item_id < 2000)
        {
            image.color = InventorySystem.instance.outLine[(int)ItemSystem.instance.weapon_dict[item_id].itemGrade];
            price = ItemSystem.instance.weapon_dict[item_id].price;
            itemInfo.GetComponentInChildren<Text>().text = ItemSystem.instance.weapon_dict[item_id].itemInfo + "\n공격력 : " + ItemSystem.instance.weapon_dict[item_id].attack.ToString() + "\n판매가격 : " + price.ToString();
        }
        else if(item_id < 3000)
        {
            image.color = InventorySystem.instance.outLine[(int)ItemSystem.instance.armor_dict[item_id].itemGrade];
            price = ItemSystem.instance.armor_dict[item_id].price;
            itemInfo.GetComponentInChildren<Text>().text = ItemSystem.instance.armor_dict[item_id].itemInfo + "\n방어력 : " + ItemSystem.instance.armor_dict[item_id].defense.ToString() + "\n판매가격 : " + price.ToString();
        }
        if (item_id < 1000)
        {
            itemNums_Text.text = "x" + item_nums.ToString();
            itemNums_Text.gameObject.SetActive(true);
        }
    }

    public void UnSetItem()
    {
        item_id = 0;
        image_child.sprite = null;
        image_child.color = new Color(1, 1, 1, 0);
        image.color = new Color(1, 1, 1, 0.39f);
        item_nums = 0;
        itemNums_Text.text = "";
        itemInfo.GetComponentInChildren<Text>().text = "";
        itemInfo.SetActive(false);
        equiped = false;
    }

    public void AddNums()
    {
        item_nums++;
        itemNums_Text.text = "x" + item_nums.ToString();
    }

    public void CloseInfo()
    {
        if (item_id == 0)
            return;
        itemInfo.SetActive(false);
    }

    public void OnPointerClick(PointerEventData eventData)
    {
        if (eventData.button == PointerEventData.InputButton.Right)
        {
            if (equiped)
            {
                UnEquip(-1);
            }
            else if (shop_item)
            {
                if(ps.gold >= price)
                {
                    if (InventorySystem.instance.PutItem(item_id))
                    {
                        ps.UpdateGold(-price);
                    }
                }
            }
            else
            {
                if (ShopWindow.instance.shop.gameObject.activeSelf)
                {
                    if (item_id < 1000)
                    {
                        ps.UpdateGold(ItemSystem.instance.consume_dict[item_id].price);
                        item_nums--;
                        itemNums_Text.text = "x" + item_nums.ToString();
                        if (item_nums == 0)
                        {
                            itemNums_Text.text = "";
                            UnSetItem();
                        }
                    }
                    else if (item_id < 2000)
                        ps.UpdateGold(ItemSystem.instance.weapon_dict[item_id].price);
                    else if (item_id < 3000)
                        ps.UpdateGold(ItemSystem.instance.armor_dict[item_id].price);
                    UnSetItem();
                }
                if(item_id < 1000)
                {
                    if(item_id == 1) // hp포션
                    {
                        ps.DrinkHpPotion(50);
                        item_nums--;
                        itemNums_Text.text = "x" + item_nums.ToString();
                        if (item_nums == 0)
                        {
                            itemNums_Text.text = "";
                            UnSetItem();
                        }
                    }
                    else if(item_id == 2)// mp포션
                    {
                        ps.DrinkMpPotion(50);
                        item_nums--;
                        itemNums_Text.text = "x" + item_nums.ToString();
                        if (item_nums == 0)
                        {
                            itemNums_Text.text = "";
                            UnSetItem();
                        }
                    }
                    return;
                }
                else if(item_id < 2000)
                {
                    int temp = item_id;
                    UnSetItem();
                    if (CharacterInfoSystem.instance.weaponSlot.item_id != 0)
                    {
                        CharacterInfoSystem.instance.weaponSlot.UnEquip(index);

                    }
                    CharacterInfoSystem.instance.weaponSlot.SetItem(temp);
                    CharacterInfoSystem.instance.weaponSlot.equiped = true;
                    ps.UpdateAttack();
                }
                else if (item_id < 3000)
                {
                    int temp = item_id;
                    UnSetItem();
                    if (CharacterInfoSystem.instance.armorSlot.item_id != 0)    
                    {
                        CharacterInfoSystem.instance.armorSlot.UnEquip(index);
                    }
                    CharacterInfoSystem.instance.armorSlot.SetItem(temp);
                    CharacterInfoSystem.instance.armorSlot.equiped = true;
                    ps.UpdateDefense();
                }
            }
        }
        else if(eventData.button == PointerEventData.InputButton.Left)
        {
            if (item_id != 0)
            {
                if (MoveItemByMouse.instance.origin != null)
                {
                    if(MoveItemByMouse.instance.origin == this)
                    {
                        MoveItemByMouse.instance.Init();
                        return;
                    }

                    if(this == CharacterInfoSystem.instance.weaponSlot)
                    {
                        if(MoveItemByMouse.instance.origin.item_id < 1000 || MoveItemByMouse.instance.origin.item_id >= 2000)
                        {
                            return;
                        }
                    }
                    if(this == CharacterInfoSystem.instance.armorSlot)
                    {
                        if (MoveItemByMouse.instance.origin.item_id < 2000)
                        {
                            return;
                        }
                    }
                    if(this == InventorySystem.instance.quickSlot1 || this == InventorySystem.instance.quickSlot2)
                    {
                        if(MoveItemByMouse.instance.origin.item_id >= 1000)
                        {
                            return;
                        }
                    }

                    int temp_item_id = MoveItemByMouse.instance.origin.item_id;
                    int temp_item_nums = MoveItemByMouse.instance.origin.item_nums;
                    MoveItemByMouse.instance.origin.UnSetItem();
                    MoveItemByMouse.instance.origin.SetItem(item_id);
                    MoveItemByMouse.instance.origin.item_nums = item_nums;
                    if(MoveItemByMouse.instance.origin.item_id < 1000)
                    {
                        MoveItemByMouse.instance.origin.itemNums_Text.text = "x" + MoveItemByMouse.instance.origin.item_nums.ToString();
                    }
                    UnSetItem();
                    SetItem(temp_item_id);
                    item_nums = temp_item_nums;
                    if(item_id < 1000)
                    {
                        itemNums_Text.text = "x" + item_nums.ToString();
                    }
                    MoveItemByMouse.instance.Init();
                }
                else
                {
                    MoveItemByMouse.instance.SetItemInfo(this);
                    InventorySystem.instance.InventoryOpen();
                }
            }
            else
            {
                if(MoveItemByMouse.instance.origin != null)
                {
                    if (this == CharacterInfoSystem.instance.weaponSlot)
                    {
                        if (MoveItemByMouse.instance.origin.item_id < 1000 || MoveItemByMouse.instance.origin.item_id >= 2000)
                        {
                            return;
                        }
                    }
                    if (this == CharacterInfoSystem.instance.armorSlot)
                    {
                        if (MoveItemByMouse.instance.origin.item_id < 2000)
                        {
                            return;
                        }
                    }
                    if (this == InventorySystem.instance.quickSlot1 || this == InventorySystem.instance.quickSlot2)
                    {
                        if (MoveItemByMouse.instance.origin.item_id >= 1000)
                        {
                            return;
                        }
                    }
                    SetItem(MoveItemByMouse.instance.origin.item_id);
                    item_nums = MoveItemByMouse.instance.origin.item_nums;
                    if (item_id < 1000)
                    {
                        itemNums_Text.text = "x" + item_nums.ToString();
                    }
                    MoveItemByMouse.instance.origin.UnSetItem();
                    MoveItemByMouse.instance.Init();
                }
            }
            ps.UpdateAttack();
            ps.UpdateDefense();
        }
    }

    public void UnEquip(int i)
    {
        if (InventorySystem.instance.PutItem(item_id, i))
        {
            UnSetItem();
            ps.UpdateAttack();
            ps.UpdateDefense();
        }
    }

    public void ShowInfo()
    {
        if (item_id == 0)
            return;
        itemInfo.SetActive(true);

    }
}