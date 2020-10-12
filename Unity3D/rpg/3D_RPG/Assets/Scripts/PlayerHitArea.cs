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
        float armor = 0;
        if(CharacterInfoSystem.instance.armorSlot.item_id != 0)
        {
            armor = ItemSystem.instance.armor_dict[CharacterInfoSystem.instance.armorSlot.item_id].defense;
        }
        if (other.transform.root.GetComponent<EnemyAttack>() != null)
        {
            transform.root.GetComponent<PlayerStatus>().OnDamage(Mathf.Max(other.transform.root.GetComponent<EnemyControl>().curPower - ps.defense - armor, 0));
            DamageText dt = Instantiate(damageText, damagePrintPos.position, Camera.main.transform.rotation);
            dt.damage = other.transform.root.GetComponent<EnemyControl>().curPower - ps.defense - armor;
        }
        else if (other.GetComponent<RangeWeapon>() != null)
        {
            transform.root.GetComponent<PlayerStatus>().OnDamage( Mathf.Max(other.GetComponent<RangeWeapon>().power - ps.defense -armor, 0));
            DamageText dt = Instantiate(damageText, damagePrintPos.position, Camera.main.transform.rotation);
            dt.damage = other.GetComponent<RangeWeapon>().power - ps.defense - armor;
        }
        else
        {
            transform.root.GetComponent<PlayerStatus>().OnDamage(Mathf.Max(other.GetComponent<SkillAttack>().power - ps.defense - armor, 0));
            DamageText dt = Instantiate(damageText, damagePrintPos.position, Camera.main.transform.rotation);
            dt.damage = other.GetComponent<SkillAttack>().power - ps.defense/ -armor;
        }
        transform.root.GetComponent<PlayerControl>().OnDamage();
    }
}
