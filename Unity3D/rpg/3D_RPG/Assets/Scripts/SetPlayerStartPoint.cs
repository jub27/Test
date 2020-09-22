using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SetPlayerStartPoint : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        PlayerControl.instance.transform.position = transform.position;
        PlayerControl.instance.SetDestination(transform.position);
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
