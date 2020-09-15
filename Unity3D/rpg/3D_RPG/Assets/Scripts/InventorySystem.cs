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
        gameObject.SetActive(false);
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

    public void PutItem(ItemSystem.ItemData item)
    {
        if (item.itemType != ItemSystem.ItemType.CONSUMED)
        {
            for (int i = 0; i < itemSlots.Length; i++)
            {
                if (itemSlots[i].empty)
                {
                    itemSlots[i].SetItem(item);
                    break;
                }
            }
        }
        else if(item.itemType == ItemSystem.ItemType.CONSUMED)
        {
            int j = -1;
            int k = 0;
            int m = -1;
            for (; k < itemSlots.Length; k++)
            {
                if (!itemSlots[k].empty && itemSlots[k].item.id == item.id)
                {
                    j = k;
                    break;
                }
                if(m == -1 && itemSlots[k].empty)
                {
                    m = k;
                }
            }
            if(j != -1)
            {
                itemSlots[j].AddNums();
            }
            else if(m != -1)
            {
                itemSlots[m].SetItem(item);
            }
        }
    }

    public void InventoryOpenClose()
    {
        if (gameObject.activeSelf == false)
            gameObject.SetActive(true);
        else
            gameObject.SetActive(false);
    }
}