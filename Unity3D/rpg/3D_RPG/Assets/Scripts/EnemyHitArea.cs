using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyHitArea : MonoBehaviour
{
    Rigidbody rigidbody;
    // Start is called before the first frame update
    void Start()
    {
        rigidbody = GetComponent<Rigidbody>();
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    private void OnTriggerEnter(Collider other)
    {
        transform.root.gameObject.GetComponent<EnemyAttack>().AttackDisable();
        transform.root.gameObject.GetComponent<CharacterStatus>().OnDamage(other.transform.root.gameObject.GetComponent<PlayerAttack>().power);
        transform.root.gameObject.GetComponent<EnemyControl>().OnDamage();
    }
}
