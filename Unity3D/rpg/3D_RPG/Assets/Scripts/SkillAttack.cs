using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SkillAttack : MonoBehaviour
{
    public float power;
    public float[] colliderActiveTime;
    public float[] sklllPower;
    public int skillType; // 1은 스킬 위치가 플레이어로부터 나가는 스킬, 2는 위치를 마우스로 지정하는 스킬
    public Collider skill_collider;
    void Start()
    {
        if (skill_collider = GetComponent<Collider>())
            skill_collider.enabled = false;
        else
            transform.Rotate(0, -5, 0);
        StartCoroutine("SkillControl");
        Destroy(gameObject, 10.0f);
    }

    IEnumerator SkillControl()
    {
        for(int i = 0; i < colliderActiveTime.Length; i++)
        {
            float waitTime;
            if (i == 0)
                waitTime = colliderActiveTime[i] - 0;
            else
                waitTime = colliderActiveTime[i] - colliderActiveTime[i-1];
            yield return new WaitForSeconds(waitTime);
            power = sklllPower[i];
            StartCoroutine("SkillColliderEnable");
        }
        yield break;
    }

    IEnumerator SkillColliderEnable()
    {
        skill_collider.enabled = true;
        yield return new WaitForSeconds(0.3f);
        skill_collider.enabled = false;
        yield break;
    }
}
