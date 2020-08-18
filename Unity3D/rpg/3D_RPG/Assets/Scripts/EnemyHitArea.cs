using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyHitArea : MonoBehaviour
{
    EnemyControl ec;
    CharacterStatus cs;
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
        transform.root.gameObject.GetComponent<EnemyAttack>().AttackDisable();
        cs.OnDamage(other.transform.root.gameObject.GetComponent<PlayerAttack>().power);
        if (!cs.dead)
        {
            ec.OnDamage();
        }
    }
}
