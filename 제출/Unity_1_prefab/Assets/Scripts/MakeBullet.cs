using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MakeBullet : MonoBehaviour
{

    public float dealay = 2;
    public float speed = 2000;
    private float curTime;
    private Rigidbody rigid;
    // Start is called before the first frame update
    void Start()
    {
        curTime = 0;
    }

    // Update is called once per frame
    void Update()
    {
        curTime += Time.deltaTime;
        if (curTime >= dealay)
        {
            GameObject obj = Instantiate(Resources.Load("Bullet"), transform) as GameObject;
            rigid = obj.GetComponent<Rigidbody>();
            rigid.AddRelativeForce(new Vector3(speed, 0, 0));
            curTime = 0;
        }
    }
}
