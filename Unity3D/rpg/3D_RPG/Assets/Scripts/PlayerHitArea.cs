using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerHitArea : MonoBehaviour
{
    GameObject player;
    public CharacterStatus cs;
    // Start is called before the first frame update
    void Start()
    {
        player = transform.root.gameObject;
        cs = transform.root.gameObject.GetComponent<CharacterStatus>();
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    private void OnTriggerEnter(Collider other)
    {
        if (cs.dead)
            return;
        transform.root.gameObject.GetComponent<PlayerAttack>().AttackDisable();
        transform.root.gameObject.GetComponent<CharacterStatus>().OnDamage(other.transform.root.gameObject.GetComponent<EnemyAttack>().power);
        transform.root.gameObject.GetComponent<CharacterMove>().OnDamage();
    }
}
