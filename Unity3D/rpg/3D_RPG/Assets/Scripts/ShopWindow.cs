using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ShopWindow : MonoBehaviour
{
    public GameObject shop;
    public static ShopWindow instance = null;

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
}
