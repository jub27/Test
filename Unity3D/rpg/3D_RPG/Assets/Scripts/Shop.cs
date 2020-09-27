using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Shop : MonoBehaviour
{
    public ItemSlot[] shop_item_list;
    private Vector3 offset;
    private bool check = false;
    static public Shop instance = null;

    void Awake()
    {
        if(instance == null)
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
        shop_item_list[0].SetItem(1);
        shop_item_list[0].shop_item = true;
        shop_item_list[1].SetItem(2);
        shop_item_list[1].shop_item = true;
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

    public void Close()
    {
        gameObject.SetActive(false);
    }

    public void Open()
    {
        gameObject.SetActive(true);
    }
}
