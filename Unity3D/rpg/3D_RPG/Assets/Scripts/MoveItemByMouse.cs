using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class MoveItemByMouse : MonoBehaviour
{
    // Start is called before the first frame update
    public static MoveItemByMouse instance = null;

    public ItemSlot origin;

    private Image img;

    private void Awake()
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
    void Start()
    {
        origin = null;
        img = GetComponent<Image>();
        gameObject.SetActive(false);
    }

    // Update is called once per frame
    void Update()
    {
        transform.position = Input.mousePosition;
    }

    public void SetItemInfo(ItemSlot itemSlot)
    {
        transform.position = Input.mousePosition;
        gameObject.SetActive(true);
        origin = itemSlot;
        img.sprite = ItemSystem.instance.item_Sprites[origin.item_id];
    }
    public void Init()
    {
        img.sprite = null;
        origin = null;
        gameObject.SetActive(false);
    }
}
