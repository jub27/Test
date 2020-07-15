using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Shot : MonoBehaviour
{
    public float speed = 1000.0f;
    private Rigidbody rigid;
    // Start is called before the first frame update

    private void Awake()
    {
        rigid = GetComponent<Rigidbody>();
    }

    void Start()
    {
        rigid.AddRelativeForce(new Vector3(speed, 0, 0));
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
