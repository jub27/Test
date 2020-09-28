using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SkillAttack : MonoBehaviour
{
    public float power;
    public float[] startTime;
    Collider skill_collider;
    void Start()
    {
        skill_collider = GetComponent<Collider>();
        StartCoroutine("SkillControl");
    }

    IEnumerator SkillControl()
    {
        for(int i = 1; i < startTime.Length; i++)
        {
            skill_collider.enabled = false;
            yield return new WaitForSeconds(startTime[i] - startTime[i-1]);
            StartCoroutine("SkillColliderEnable");
        }
        yield break;
    }

    IEnumerator SkillColliderEnable()
    {
        skill_collider.enabled = true;
        yield return new WaitForSeconds(0.5f);
        skill_collider.enabled = false;
        yield break;
    }
}
