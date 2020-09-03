using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerHitArea : MonoBehaviour
{
    GameObject player;
    private PlayerStatus ps;
    public DamageText damageText;
    public Transform damagePrintPos;
    // Start is called before the first frame update
    void Start()
    {
        player = transform.root.gameObject;
        ps = transform.root.gameObject.GetComponent<PlayerStatus>();
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    private void OnTriggerEnter(Collider other)
    {
        if (ps.dead)
            return;
        transform.root.GetComponent<PlayerAttack>().AttackDisable();
        if (other.transform.root.GetComponent<EnemyAttack>() != null)
        {
            transform.root.GetComponent<PlayerStatus>().OnDamage(other.transform.root.GetComponent<EnemyAttack>().power);
            DamageText dt = Instantiate(damageText, damagePrintPos.position, Camera.main.transform.rotation);
            dt.damage = other.transform.root.GetComponent<EnemyAttack>().power;
        }
        else if (other.GetComponent<RangeWeapon>() != null)
        {
            transform.root.GetComponent<PlayerStatus>().OnDamage(other.GetComponent<RangeWeapon>().power);
            DamageText dt = Instantiate(damageText, damagePrintPos.position, Camera.main.transform.rotation);
            dt.damage = other.GetComponent<RangeWeapon>().power;
        }
        else
        {
            transform.root.GetComponent<PlayerStatus>().OnDamage(other.GetComponent<ParticleCollider>().power);
            DamageText dt = Instantiate(damageText, damagePrintPos.position, Camera.main.transform.rotation);
            dt.damage = other.GetComponent<ParticleCollider>().power;
        }
        transform.root.GetComponent<PlayerControl>().OnDamage();
    }
}
