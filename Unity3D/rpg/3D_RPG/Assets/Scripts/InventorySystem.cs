using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
public class InventorySystem : MonoBehaviour
{
    static public InventorySystem instance = null;

    private Vector3 offset;
    private bool check = false;

    public Color[] outLine;
    public ItemSlot[] itemSlots;
    public ItemSlot quickSlot1;
    public ItemSlot quickSlot2;
    private void Awake()
    {
        if (instance == null)
        {
            instance = this;
            for (int i = 0; i < itemSlots.Length; i++)
            {
                itemSlots[i].index = i;
            }
            outLine = new Color[4];
            outLine[0] = new Color(1, 1, 1, 0.5f);
            outLine[1] = new Color(0, 0.4688f, 1, 0.5f);
            outLine[2] = new Color(1, 0, 0.9584f, 0.5f);
            outLine[3] = new Color(1, 0.5854f, 0, 0.5f);
        }
        else
        {
            Destroy(gameObject);
        }
    }
    // Start is called before the first frame update
    void Start()
    {
        for (int i = 0; i < GameManager.instance.cur_user_data.inventory.Length; i++)
        {
            if (GameManager.instance.cur_user_data.inventory[i].item_id != 0)
            {
                PutItem(GameManager.instance.cur_user_data.inventory[i].item_id, i);
            }
        }
        if (GameManager.instance.cur_user_data.quickSlot1.item_id != 0)
        {
            quickSlot1.SetItem(GameManager.instance.cur_user_data.quickSlot1.item_id);
            quickSlot1.item_nums = GameManager.instance.cur_user_data.quickSlot1.item_nums;
            quickSlot1.itemNums_Text.text = "x" + quickSlot1.item_nums.ToString();
        }
        if (GameManager.instance.cur_user_data.quickSlot2.item_id != 0)
        {
            quickSlot2.SetItem(GameManager.instance.cur_user_data.quickSlot2.item_id);
            quickSlot2.item_nums = GameManager.instance.cur_user_data.quickSlot2.item_nums;
            quickSlot2.itemNums_Text.text = "x" + quickSlot2.item_nums.ToString();
        }
        gameObject.SetActive(false);
    }

    // Update is called once per frame
    void Update()
    {
        if (GetComponentInChildren<DragWindow>().isMoving)
        {
            if (check == false)
            {
                offset = transform.position - GetComponentInChildren<DragWindow>().baseMousePosition;
                check = true;
            }
            else
            {
                transform.position = Input.mousePosition + offset;
            }
        }
        else
        {
            check = false;
        }
    }

    public bool PutItem(int item_id, int index = -1)
    {
        if (index != -1)
        {
            itemSlots[index].SetItem(item_id);
            return true;
        }
        if (item_id >= 1000)
        {
            for (int i = 0; i < itemSlots.Length; i++)
            {
                if (itemSlots[i].item_id == 0)
                {
                    itemSlots[i].SetItem(item_id);
                    return true;
                }
            }
        }
        else if (item_id < 1000)
        {
            int j = -1;
            int k = 0;
            int m = -1;
            if(quickSlot1.item_id == item_id)
            {
                quickSlot1.AddNums();
                return true;
            }
            if(quickSlot2.item_id == item_id)
            {
                quickSlot2.AddNums();
                return true;
            }
            for (; k < itemSlots.Length; k++)
            {
                if (itemSlots[k].item_id != 0 && itemSlots[k].item_id == item_id)
                {
                    j = k;
                    break;
                }
                if (m == -1 && itemSlots[k].item_id == 0)
                {
                    m = k;
                }
            }
            if (j != -1)
            {
                itemSlots[j].AddNums();
                return true;
            }
            else if (m != -1)
            {
                itemSlots[m].SetItem(item_id);
                return true;
            }
        }
        return false;
    }


    public void InventoryOpen()
    {
        gameObject.SetActive(true);
    }

    public void InventoryClose()
    {
        gameObject.SetActive(false);
        MoveItemByMouse.instance.Init();
    }

    public void Sort()
    {
        List<KeyValuePair<int, int>> temp = new List<KeyValuePair<int, int>>();
        for (int i = 0; i < itemSlots.Length; i++)
        {
            temp.Add(new KeyValuePair<int, int>(itemSlots[i].item_id, itemSlots[i].item_nums));
        }
        temp.Sort((a, b) => -(a.Key.CompareTo(b.Key)));
        for (int i = 0; i < itemSlots.Length; i++)
        {
            itemSlots[i].UnSetItem();
            if (temp[i].Key != 0)
            {
                itemSlots[i].SetItem(temp[i].Key);
                if (temp[i].Key < 1000)
                {
                    itemSlots[i].item_nums = temp[i].Value;
                    itemSlots[i].itemNums_Text.text = "x" + itemSlots[i].item_nums.ToString();
                }
            }
        }
    }
}