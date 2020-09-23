using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class InventorySystem : MonoBehaviour
{
    static public InventorySystem instance = null;

    private Vector3 offset;
    private bool check = false;

    public Color[] outLine;
    public ItemSlot[] itemSlots;

    private void Awake()
    {
        if (instance == null)
        {
            instance = this;
        }
        else
        {
            Destroy(gameObject);
        }
    }
    // Start is called before the first frame update
    void Start()
    {
        gameObject.SetActive(false);
        for(int i = 0; i < itemSlots.Length; i++)
        {
            itemSlots[i].index = i;
        }
        outLine = new Color[4];
        outLine[0] = new Color(1, 1, 1, 0.5f);
        outLine[1] = new Color(0, 0.4688f, 1, 0.5f);
        outLine[2] = new Color(1, 0, 0.9584f, 0.5f);
        outLine[3] = new Color(1, 0.5854f, 0, 0.5f);
        if (GameManager.instance.is_loaded)
        {
            for (int i = 0; i < GameManager.instance.load_data.inventory.Length; i++)
            {
                if (GameManager.instance.load_data.inventory[i].item_id != 0)
                {
                    PutItem(GameManager.instance.load_data.inventory[i].item_id, i);
                }
            }
        }

    }

    // Update is called once per frame
    void Update()
    {
        if (GetComponentInChildren<DragWindow>().isMoving)
        {
            if(check == false)
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
        else if(item_id < 1000)
        {
            int j = -1;
            int k = 0;
            int m = -1;
            for (; k < itemSlots.Length; k++)
            {
                if (itemSlots[k].item_id != 0 && itemSlots[k].item_id == item_id)
                {
                    j = k;
                    break;
                }
                if(m == -1 && itemSlots[k].item_id == 0)
                {
                    m = k;
                }
            }
            if(j != -1)
            {
                itemSlots[j].AddNums();
                return true;
            }
            else if(m != -1)
            {
                itemSlots[m].SetItem(item_id);
                return true;
            }
        }
        return false;
    }


    public void InventoryOpenClose()
    {
        if (gameObject.activeSelf == false)
            gameObject.SetActive(true);
        else
            gameObject.SetActive(false);
    }
}