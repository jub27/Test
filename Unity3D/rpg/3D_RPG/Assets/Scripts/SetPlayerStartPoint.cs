using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SetPlayerStartPoint : MonoBehaviour
{
    // Start is called before the first frame update
    private void Awake()
    {
        PlayerControl.instance.transform.position = transform.position;
        PlayerControl.instance.transform.rotation = transform.rotation;
        PlayerControl.instance.SetDestination(transform.position);
    }

    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
