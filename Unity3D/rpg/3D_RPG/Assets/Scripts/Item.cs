using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;

public class Item : MonoBehaviour
{
    public GameObject[] Effects;
    private Rigidbody rigid;
    public int item_id;
    private TextMeshPro tm;

    // Start is called before the first frame update
    void Start()
    {
        rigid = GetComponent<Rigidbody>();
        tm = GetComponentInChildren<TextMeshPro>();
        int itemType = (int)Random.Range(0, 3);
        int random = -1;
        int itemGrade = -1;
        if(itemType == (int)ItemSystem.ItemType.CONSUMED)
        {
            random = (int)Random.Range(0, ItemSystem.instance.itemList.consumeData_list.Length);
            item_id = (ItemSystem.instance.itemList.consumeData_list[random].id);
            itemGrade = ItemSystem.instance.consume_dict[item_id].itemGrade;
            tm.text = ItemSystem.instance.consume_dict[item_id].name;
        }
        else if(itemType == (int)ItemSystem.ItemType.WEAPON)
        {
            random = (int)Random.Range(0, ItemSystem.instance.itemList.weaponData_list.Length);
            item_id = (ItemSystem.instance.itemList.weaponData_list[random].id);
            itemGrade = ItemSystem.instance.weapon_dict[item_id].itemGrade;
            tm.text = ItemSystem.instance.weapon_dict[item_id].name;
        }
        else
        {
            random = (int)Random.Range(0, ItemSystem.instance.itemList.armorData_list.Length);
            item_id = (ItemSystem.instance.itemList.armorData_list[random].id);
            itemGrade = ItemSystem.instance.armor_dict[item_id].itemGrade;
            tm.text = ItemSystem.instance.armor_dict[item_id].name;
        }

        if(itemGrade == (int)ItemSystem.ItemGrade.LEGENDARY)
        {
            tm.color = new Color(255, 111, 0, 255);
        }
        else if(itemGrade == (int)ItemSystem.ItemGrade.EPIC)
        {
            tm.color = new Color(112, 43, 255, 255);
        }
        else if(itemGrade == (int)ItemSystem.ItemGrade.UNCOMMON)
        {
            tm.color = new Color(0, 86, 255, 255);
        }
        else
        {
            tm.color = new Color(255, 241, 184, 125);
        }
        GameObject temp =  Instantiate(Effects[itemGrade], transform.position, transform.rotation);
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
            InventorySystem.instance.PutItem(item_id);
            Destroy(gameObject);
        }
    }
}
