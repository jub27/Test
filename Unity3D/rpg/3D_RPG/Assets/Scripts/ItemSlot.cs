using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;
using System.Text;
public class ItemSlot : MonoBehaviour, IPointerClickHandler
{
    public int item_id;
    public int item_nums;
    public bool equiped;


    public Image image_child;
    private Image image;
    private Text itemNums_Text;
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
            itemInfo.GetComponentInChildren<Text>().text = ItemSystem.instance.consume_dict[item_id].itemInfo;
        }
        else if(item_id < 2000)
        {
            image.color = InventorySystem.instance.outLine[(int)ItemSystem.instance.weapon_dict[item_id].itemGrade];
            itemInfo.GetComponentInChildren<Text>().text = ItemSystem.instance.weapon_dict[item_id].itemInfo;
        }
        else if(item_id < 3000)
        {
            image.color = InventorySystem.instance.outLine[(int)ItemSystem.instance.armor_dict[item_id].itemGrade];
            itemInfo.GetComponentInChildren<Text>().text = ItemSystem.instance.armor_dict[item_id].itemInfo;
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
        if (item_id == 0)
        {
            return;
        }
        if (eventData.button == PointerEventData.InputButton.Right)
        {
            if (equiped)
            {
                UnEquip(-1);
            }
            else
            {
                if(item_id < 1000)
                {
                    if(item_id == 1) // hp포션
                    {
                        ps.DrinkHpPotion(30);
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
                        ps.DrinkMpPotion(30);
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
                    ps.UpdateAttack(ItemSystem.instance.weapon_dict[temp].attack);
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
                    ps.UpdateDefense(ItemSystem.instance.armor_dict[temp].defense);
                }
            }
        }
    }

    public void UnEquip(int i)
    {
        if (InventorySystem.instance.PutItem(item_id, i))
        {
            if(item_id < 2000)
            {
                ps.UpdateAttack(-(ItemSystem.instance.weapon_dict[item_id].attack));
            }
            else if (item_id < 3000)
            {
                ps.UpdateDefense(-(ItemSystem.instance.armor_dict[item_id].defense));
            }
            UnSetItem();
        }
    }

    public void ShowInfo()
    {
        if (item_id == 0)
            return;
        itemInfo.SetActive(true);

    }
}