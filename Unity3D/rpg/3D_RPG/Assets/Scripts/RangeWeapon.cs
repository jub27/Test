using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RangeWeapon : MonoBehaviour
{
    private Transform target;
    private Vector3 dir;
    public float speed = 25.0f;
    public float power = 5.0f;
    // Start is called before the first frame update
    void Start()
    {
        target = FindObjectOfType<PlayerAttack>().transform;
        dir = target.position - transform.position;
        dir.y += 1.0f;
        dir.Normalize();
        Destroy(this.gameObject, 5);
    }

    // Update is called once per frame
    void Update()
    {
        transform.position += dir * speed * Time.deltaTime;
    }

    private void OnTriggerEnter(Collider other)
    {
        Destroy(this.gameObject);
    }
}
