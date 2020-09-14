using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ItemSlot : MonoBehaviour
{
    private ItemSystem.ItemData item;
    private Image image;
    public bool empty;
    // Start is called before the first frame update
    private void Start()
    {
        image = GetComponent<Image>();
        empty = true;
    }

    public void SetItem(ItemSystem.ItemData item)
    {
        this.item = item;
        image.sprite = ItemSystem.instance.item_Sprites[item.id];
        image.material = InventorySystem.instance.outLine[(int)item.itemGrade];
        empty = false;
    }

}