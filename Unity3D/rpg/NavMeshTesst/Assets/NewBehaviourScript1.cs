using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class NewBehaviourScript1 : MonoBehaviour
{
    public GameObject pref;
    // Start is called before the first frame update
    void Start()
    {
        Instantiate(pref, transform.position, transform.rotation);
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
