using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Jump : MonoBehaviour
{
    public Rigidbody rigidbody;

    void Start()
    {
        rigidbody.AddForce(0f, 500f, 0f);
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
