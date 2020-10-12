using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyAttack : MonoBehaviour
{
    public GameObject weapon;
    public GameObject weapon2;
    Collider cc;
    Collider cc2;
    // Start is called before the first frame update
    void Start()
    {
        cc = weapon.GetComponent<Collider>();
        if(weapon2 != null)
        {
            cc2 = weapon2.GetComponent<Collider>();
        }
    }

    private void OnTriggerEnter(Collider other)
    {
        cc.enabled = false;
    }

    public void AttackEnable()
    {
        cc.enabled = true;
    }

    public void AttackEnable2()
    {
        cc2.enabled = true;
    }

    public void AttackDisable()
    {
        cc.enabled = false;
    }
    public void AttackDisable2()
    {
        cc2.enabled = false;
    }
}
