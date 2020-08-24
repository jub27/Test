using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerHitArea : MonoBehaviour
{
    GameObject player;
    private CharacterStatus cs;
    public DamageText damageText;
    public Transform damagePrintPos;
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
        if (other.transform.root.gameObject.GetComponent<EnemyAttack>() != null)
        {
            transform.root.gameObject.GetComponent<CharacterStatus>().OnDamage(other.transform.root.gameObject.GetComponent<EnemyAttack>().power);
            DamageText dt = Instantiate(damageText, damagePrintPos.position, Camera.main.transform.rotation);
            dt.damage = other.transform.root.gameObject.GetComponent<EnemyAttack>().power;
        }
        else
        {
            transform.root.gameObject.GetComponent<CharacterStatus>().OnDamage(other.gameObject.GetComponent<RangeWeapon>().power);
            DamageText dt = Instantiate(damageText, damagePrintPos.position, Camera.main.transform.rotation);
            dt.damage = other.gameObject.GetComponent<RangeWeapon>().power;
        }
        transform.root.gameObject.GetComponent<CharacterMove>().OnDamage();
    }

    private void OnParticleCollision(GameObject other)
    {
        if (cs.dead)
            return;
        transform.root.gameObject.GetComponent<PlayerAttack>().AttackDisable();
        transform.root.gameObject.GetComponent<CharacterStatus>().OnDamage(other.GetComponent<ParticleCollider>().power);
        DamageText dt = Instantiate(damageText, damagePrintPos.position, Camera.main.transform.rotation);
        dt.damage = other.GetComponent<ParticleCollider>().power;
        transform.root.gameObject.GetComponent<CharacterMove>().OnDamage();
    }
}
