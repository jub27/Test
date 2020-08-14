using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerAttack : MonoBehaviour
{
    public GameObject Weapon;
    CapsuleCollider cc;
    // Start is called before the first frame update
    void Start()
    {
        cc = Weapon.GetComponent<CapsuleCollider>();
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
        cc.enabled = true;
    }

    public void AttackDisable()
    {
        cc.enabled = false;
    }

}
