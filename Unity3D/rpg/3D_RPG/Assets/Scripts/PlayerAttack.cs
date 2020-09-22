using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerAttack : MonoBehaviour
{
    public GameObject weapon;
    CapsuleCollider cc;
    Animator playerAnimator;
    public bool combo = false;
    public bool attakEnd = true;
    // Start is called before the first frame update
    void Start()
    {
        cc = weapon.GetComponent<CapsuleCollider>();
        playerAnimator = GetComponent<Animator>();
        cc.enabled = false;
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
    public void AttackEnd()
    {
 
    }

    public void ComboStart()
    {
        combo = true;
        playerAnimator.SetBool("Combo", false);
    }

    public void ComboEnd()
    {
        combo = false;
    }

    public void ResetAttack()
    {
        attakEnd = true;
        combo = false;
        playerAnimator.SetBool("Attack", false);
        playerAnimator.SetBool("Combo", false);
    }
}
