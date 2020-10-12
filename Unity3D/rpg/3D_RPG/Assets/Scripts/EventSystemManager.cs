using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EventSystemManager : MonoBehaviour
{

    static public GameObject instance = null;

    private void Awake()
    {
        if (instance == null)
        {
            instance = this.gameObject;
            DontDestroyOnLoad(gameObject);
            GameManager.instance.dontDestroyObjectList.Add(gameObject);
        }
        else
        {
            Destroy(this.gameObject);
        }
    }
}
