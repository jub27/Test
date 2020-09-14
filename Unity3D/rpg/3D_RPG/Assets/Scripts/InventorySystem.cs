using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class InventorySystem : MonoBehaviour
{
    static public InventorySystem instance = null;

    private Vector3 offset;
    private bool check = false;

    public Material[] outLine;
    public ItemSlot[] itemSlots;

    private void Awake()
    {
        if (instance == null)
        {
            instance = this;

            outLine = new Material[4];
            outLine[0] = Resources.Load("Common", typeof(Material)) as Material;
            outLine[1] = Resources.Load("UnCommon", typeof(Material)) as Material;
            outLine[2] = Resources.Load("Epic", typeof(Material)) as Material;
            outLine[3] = Resources.Load("Legendary", typeof(Material)) as Material;
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
        for(int i = 0; i < itemSlots.Length; i++)
        {
            if (itemSlots[i].empty)
            {
                itemSlots[i].SetItem(item);
                break;
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