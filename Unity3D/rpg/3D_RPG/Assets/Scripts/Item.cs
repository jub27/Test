using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;

public class Item : MonoBehaviour
{
    public GameObject[] Effects;
    private Rigidbody rigid;
    private ItemSystem.ItemData itemData;
    private TextMeshPro tm;

    // Start is called before the first frame update
    void Start()
    {
        rigid = GetComponent<Rigidbody>();
        tm = GetComponentInChildren<TextMeshPro>();
        int random = (int)Random.Range(0, 100);

        itemData = (ItemSystem.ItemData)(ItemSystem.instance.itemList[random]);

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
}
