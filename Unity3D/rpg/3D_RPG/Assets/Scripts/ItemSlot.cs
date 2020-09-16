using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;
using System.Text;
public class ItemSlot : MonoBehaviour
{
    public ItemSystem.ItemData item;
    public Image image_child;
    private Image image;
    private Text itemNums_Text;
    public GameObject itemInfo;
    private int itemNums;
    public bool empty;

    // Start is called before the first frame update
    private void Start()
    {
        image = GetComponent<Image>();
        itemNums_Text = GetComponentInChildren<Text>();
        itemNums = 0;
        empty = true;
    }

    public void SetItem(ItemSystem.ItemData item)
    {
        this.item = item;
        image_child.sprite = ItemSystem.instance.item_Sprites[item.id];
        image_child.color = new Color(255, 255, 255, 255);
        image.color = InventorySystem.instance.outLine[(int)item.itemGrade];
        itemNums = 1;
        itemInfo.GetComponentInChildren<Text>().text = item.itemInfo;
        print(item.itemInfo);
        if (item.itemType == ItemSystem.ItemType.CONSUMED)
        {
            itemNums_Text.text = "x" + itemNums.ToString();
            itemNums_Text.gameObject.SetActive(true);
        }
        empty = false;
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


    public void ShowInfo()
    {
        if (empty)
            return;
        itemInfo.SetActive(true);

    }
}