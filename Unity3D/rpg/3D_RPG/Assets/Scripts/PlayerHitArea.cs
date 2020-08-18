using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerHitArea : MonoBehaviour
{
    GameObject player;
    // Start is called before the first frame update
    void Start()
    {
        player = transform.root.gameObject;
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    private void OnTriggerEnter(Collider other)
    {
        transform.root.gameObject.GetComponent<PlayerAttack>().AttackDisable();
        transform.root.gameObject.GetComponent<CharacterStatus>().OnDamage(other.transform.root.gameObject.GetComponent<EnemyAttack>().power);
        transform.root.gameObject.GetComponent<CharacterMove>().OnDamage();
    }
}
