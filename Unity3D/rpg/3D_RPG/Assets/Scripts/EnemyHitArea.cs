using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyHitArea : MonoBehaviour
{
    EnemyControl ec;
    CharacterStatus cs;
    public DamageText damageText;
    public Transform damagePrintPos;
    // Start is called before the first frame update
    void Start()
    {
        ec = transform.root.gameObject.GetComponent<EnemyControl>();
        cs = transform.root.gameObject.GetComponent<CharacterStatus>();
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    private void OnTriggerEnter(Collider other)
    {
        ec.SetChasing(other.transform.root.transform);
        if(transform.root.gameObject.GetComponent<EnemyAttack>() != null)
            transform.root.gameObject.GetComponent<EnemyAttack>().AttackDisable();
        cs.OnDamage(other.transform.root.gameObject.GetComponent<PlayerAttack>().power);
        DamageText dt = Instantiate(damageText, damagePrintPos.position, Camera.main.transform.rotation);
        dt.damage = other.transform.root.gameObject.GetComponent<PlayerAttack>().power;
        if (!cs.dead)
        {
            ec.OnDamage();
        }
        else
        {
            GetComponent<Collider>().enabled = false;
        }
    }
}
