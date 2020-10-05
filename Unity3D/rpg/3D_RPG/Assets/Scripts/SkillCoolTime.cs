using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
public class SkillCoolTime : MonoBehaviour
{
    public Image img;
    public float coolTime;
    private float curTime;
    public bool skill_cool;
    // Start is called before the first frame update
    void Start()
    {
        curTime = coolTime;
        skill_cool = true;
    }

    // Update is called once per frame
    void Update()
    {
        if ( curTime < coolTime)
        {
            curTime += Time.deltaTime;
            img.fillAmount = curTime / coolTime;
            skill_cool = false;
        }
        else
        {
            curTime = coolTime;
            img.fillAmount = 1.0f;
            skill_cool = true;
        }
    }

    public void CoolStart()
    {
        curTime = 0;
        skill_cool = false;
    }
}
