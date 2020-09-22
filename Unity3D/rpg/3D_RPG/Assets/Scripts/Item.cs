using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;

public class Item : MonoBehaviour
{
    public GameObject[] Effects;
    private Rigidbody rigid;
    public ItemData itemData;
    private TextMeshPro tm;

    // Start is called before the first frame update
    void Start()
    {
        rigid = GetComponent<Rigidbody>();
        tm = GetComponentInChildren<TextMeshPro>();
        int itemType = (int)Random.Range(0, 3);
        int random = -1;
        if(itemType == (int)ItemSystem.ItemType.CONSUMED)
        {
            random = (int)Random.Range(0, ItemSystem.instance.itemList.consumeData_list.Length);
            itemData = (ItemSystem.instance.itemList.consumeData_list[random]);
        }
        else if(itemType == (int)ItemSystem.ItemType.WEAPON)
        {
            random = (int)Random.Range(0, ItemSystem.instance.itemList.weaponData_list.Length);
            itemData = (ItemSystem.instance.itemList.weaponData_list[random]);
        }
        else
        {
            random = (int)Random.Range(0, ItemSystem.instance.itemList.armorData_list.Length);
            itemData = (ItemSystem.instance.itemList.armorData_list[random]);
        }

        if(itemData.itemGrade == ItemSystem.ItemGrade.LEGENDARY)
        {
            tm.color = new Color(255, 111, 0, 255);
        }
        else if(itemData.itemGrade == ItemSystem.ItemGrade.EPIC)
        {
            tm.color = new Color(112, 43, 255, 255);
        }
        else if(itemData.itemGrade == ItemSystem.ItemGrade.UNCOMMON)
        {
            tm.color = new Color(0, 86, 255, 255);
        }
        else
        {
            tm.color = new Color(255, 241, 184, 125);
        }
        tm.text = itemData.name;
        GameObject temp =  Instantiate(Effects[(int)itemData.itemGrade], transform.position, transform.rotation);
        temp.transform.parent = transform;
        float x = Random.Range(-4, 4);
        float y = Random.Range(10, 15);
        float z = Random.Range(-4, 4);
        rigid.AddForce(new Vector3(x, y, z), ForceMode.Impulse);
    }

    // Update is called once per frame
    void Update()
    {
        tm.gameObject.transform.forward = Camera.main.transform.forward;
    }

    private void OnCollisionEnter(Collision collision)
    {
        if(collision.gameObject.tag == "Player")
        {
            InventorySystem.instance.PutItem(itemData);
            Destroy(gameObject);
        }
    }
}
