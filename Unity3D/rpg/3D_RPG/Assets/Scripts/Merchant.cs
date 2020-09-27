using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Merchant : MonoBehaviour
{
    private ShopWindow shop;
    private GameObject Player;
    // Start is called before the first frame update
    void Start()
    {
        Player = GameObject.Find("Player");
        shop = GameObject.Find("ShopWindow").GetComponent<ShopWindow>();
    }

    // Update is called once per frame
    void Update()
    {
        if((Player.transform.position - transform.position).magnitude < 1.5){
            OpenShop();
        }
        else
        {
            CloseShop();
        }
    }

    void OpenShop()
    {
        shop.shop.SetActive(true);
    }

    void CloseShop()
    {
        shop.shop.SetActive(false);
    }
}
