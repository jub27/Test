using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;
using System.Text;
public class ItemSlot : MonoBehaviour, IPointerClickHandler
{
    public ItemData item;
    public Image image_child;
    private Image image;
    private Text itemNums_Text;
    public GameObject itemInfo;
    private int itemNums;
    public bool empty = true;
    public bool equiped;
    public int index;
    static int count = 0;
    private PlayerStatus ps;
    private void Awake()
    {
        image = GetComponent<Image>();
        itemNums_Text = GetComponentInChildren<Text>();
        ps = GameObject.Find("Player").GetComponent<PlayerStatus>();
        itemNums = 0;
        empty = true;
        equiped = false;
    }

    public void SetItem(ItemData item)
    {
        this.item = item;
        image_child.sprite = ItemSystem.instance.item_Sprites[item.id];
        image_child.color = new Color(1, 1, 1, 1);
        image.color = InventorySystem.instance.outLine[(int)item.itemGrade];
        itemNums = 1;
        itemInfo.GetComponentInChildren<Text>().text = item.itemInfo;
        if (item.itemType == ItemSystem.ItemType.CONSUMED)
        {
            itemNums_Text.text = "x" + itemNums.ToString();
            itemNums_Text.gameObject.SetActive(true);
        }
        empty = false;
    }

    public void UnSetItem()
    {
        if(item.itemType == ItemSystem.ItemType.WEAPON)
        {
            ps.attack -= (item as WeaponData).attack;
        }
        else if (item.itemType == ItemSystem.ItemType.ARMOR)
        {
            ps.defense -= (item as ArmorData).defense;
        }
        item = new ItemData(0, "", 0, 0, "");
        image_child.sprite = null;
        image_child.color = new Color(1, 1, 1, 0);
        image.color = new Color(1, 1, 1, 0.39f);
        itemNums = 0;
        itemInfo.GetComponentInChildren<Text>().text = "";
        itemInfo.SetActive(false);
        equiped = false;
        empty = true;
    }

    public void AddNums()
    {
        itemNums++;
        itemNums_Text.text = "x" + itemNums.ToString();
    }

    public void CloseInfo()
    {
        if (empty)
            return;
        itemInfo.SetActive(false);
    }

    public void OnPointerClick(PointerEventData eventData)
    {
        if (empty)
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
                if(item.itemType == ItemSystem.ItemType.CONSUMED)
                {
                    if(item.id == 1) // hp포션
                    {
                        ps.DrinkHpPotion(30);
                        itemNums--;
                        if(itemNums == 0)
                        {
                            UnSetItem();
                        }
                    }
                    else if(item.id == 2)// mp포션
                    {
                        ps.DrinkMpPotion(30);
                        itemNums--;
                        if (itemNums == 0)
                        {
                            UnSetItem();
                        }
                    }
                    return;
                }
                else if(item.itemType == ItemSystem.ItemType.ARMOR)
                {
                    ItemData temp = item;
                    UnSetItem();
                    if (CharacterInfoSystem.instance.armorSlot.empty == false)
                    {
                        ArmorData adata = CharacterInfoSystem.instance.armorSlot.item as ArmorData;
                        ps.UpdateDefense(-adata.defense);
                        CharacterInfoSystem.instance.armorSlot.UnEquip(index);
                    }
                    CharacterInfoSystem.instance.armorSlot.SetItem(temp);
                    CharacterInfoSystem.instance.armorSlot.equiped = true;
                    ps.UpdateDefense((temp as ArmorData).defense);
                }
                else if (item.itemType == ItemSystem.ItemType.WEAPON)
                {
                    ItemData temp = item;
                    UnSetItem();
                    if (CharacterInfoSystem.instance.weaponSlot.empty == false)
                    {
                        WeaponData wdata = CharacterInfoSystem.instance.weaponSlot.item as WeaponData;
                        ps.UpdateAttack(-wdata.attack);
                        CharacterInfoSystem.instance.weaponSlot.UnEquip(index);

                    }
                    CharacterInfoSystem.instance.weaponSlot.SetItem(temp);
                    CharacterInfoSystem.instance.weaponSlot.equiped = true;
                    ps.UpdateAttack((temp as WeaponData).attack);
                }
            }
        }
    }

    public void UnEquip(int i)
    {
        if (InventorySystem.instance.PutItem(item, i))
        {
            UnSetItem();
        }
    }

    public void ShowInfo()
    {
        if (empty)
            return;
        itemInfo.SetActive(true);

    }
}