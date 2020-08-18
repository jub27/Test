using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyAttack : MonoBehaviour
{
    public GameObject weapon;
    Collider cc;
    public float power = 5;
    // Start is called before the first frame update
    void Start()
    {
        cc = weapon.GetComponent<Collider>();
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    private void OnTriggerEnter(Collider other)
    {
        cc.enabled = false;
    }

    public void AttackEnable()
    {
        print("AE");
        cc.enabled = true;
    }

    public void AttackDisable()
    {
        print("AD");
        cc.enabled = false;
    }
}
