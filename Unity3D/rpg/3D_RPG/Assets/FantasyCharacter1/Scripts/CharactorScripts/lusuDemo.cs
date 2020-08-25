using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class dianweiDemo : MonoBehaviour {

    public GameObject attackBullet;
    public GameObject magicBullet;
    public GameObject magic2Bullet;
    public GameObject ultimateBullet;
    public GameObject damageEffect1;
    public GameObject damageEffect2;
    public GameObject damageEffect3;
	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		
	}

    IEnumerator delayBullet()
    {
        yield return new WaitForSeconds(2f);
        AttackedController1 c = GameObject.Find("bigzhangjiao (1)").GetComponent<AttackedController1>();
        c.attacked();
    }

    IEnumerator delayBullet1()
    {
        int count = 5;
        float angle = -0 / 4f * 10f;
        for (int i = 0; i < count; i++)
        {
            GameObject obj = GameObject.Instantiate(attackBullet);
            PosBullet1 bullet = obj.GetComponent<PosBullet1>();
            bullet.player = transform;
            bullet.startPos = transform.position + new Vector3(0f, 0.01f, 0f);
            bullet.tarPos = MathUtil1.calcTargetPosByRotation(transform, angle + i * 10f, 10f);
            bullet.effectObj = damageEffect1;
            bullet.bulleting();
            yield return new WaitForSeconds(0.015f);
            if (i % 6 == 0)
            {
                AttackedController1 c = GameObject.Find("bigzhangjiao (1)").GetComponent<AttackedController1>();
                c.attacked();
                if (damageEffect2 != null)
                {
                    GameObject obj1 = GameObject.Instantiate(damageEffect2);
                    ParticlesEffect1 effect = obj1.AddComponent<ParticlesEffect1>();
                    Transform target = GameObject.Find("bigzhangjiao (1)").transform;
                    effect.transform.position = MathUtil1.findChild(target, "attackedPivot").position;
                    effect.play();
                }
            }
        }
    }

    IEnumerator delayBullet2()
    {
        int count = 10;
        float angle = -5 / 2f * 10f;
        for (int i = 0; i < count; i++)
        {
            GameObject obj = GameObject.Instantiate(magicBullet);
            PosBullet1 bullet = obj.GetComponent<PosBullet1>();
            bullet.player = transform;
            bullet.startPos = transform.position + new Vector3(0f, 0.01f, 0f);
            bullet.tarPos = MathUtil1.calcTargetPosByRotation(transform, angle + i * 10f, 10f);
            bullet.effectObj = damageEffect1;
            bullet.bulleting();
            yield return new WaitForSeconds(0.015f);
            if (i % 6 == 0)
            {
                AttackedController1 c = GameObject.Find("bigzhangjiao (1)").GetComponent<AttackedController1>();
                c.attacked();
                if (damageEffect2 != null)
                {
                    GameObject obj1 = GameObject.Instantiate(damageEffect2);
                    ParticlesEffect1 effect = obj1.AddComponent<ParticlesEffect1>();
                    Transform target = GameObject.Find("bigzhangjiao (1)").transform;
                    effect.transform.position = MathUtil1.findChild(target, "attackedPivot").position;
                    effect.play();
                }
            }
        }
    }

    IEnumerator delayBullet3()
    {
        int count = 20;
        float angle = -10 / 2f * 10f;
        for (int i = 0; i < count; i++)
        {
            GameObject obj = GameObject.Instantiate(ultimateBullet);
            PosBullet1 bullet = obj.GetComponent<PosBullet1>();
            bullet.player = transform;
            bullet.startPos = transform.position + new Vector3(0f, 0.01f, 0f);
            bullet.tarPos = MathUtil1.calcTargetPosByRotation(transform, angle + i * 10f, 10f);
            bullet.effectObj = damageEffect1;
            bullet.bulleting();
            yield return new WaitForSeconds(0.015f);
            if (i % 6 == 0)
            {
                AttackedController1 c = GameObject.Find("bigzhangjiao (1)").GetComponent<AttackedController1>();
                c.attacked();
                if (damageEffect2 != null)
                {
                    GameObject obj1 = GameObject.Instantiate(damageEffect2);
                    ParticlesEffect1 effect = obj1.AddComponent<ParticlesEffect1>();
                    Transform target = GameObject.Find("bigzhangjiao (1)").transform;
                    effect.transform.position = MathUtil1.findChild(target, "attackedPivot").position;
                    effect.play();
                }
            }
        }
    }
    void preAction(string actionName)
    {
        AttackedController1 c = GameObject.Find("bigzhangjiao (1)").GetComponent<AttackedController1>();
        string[] arr = actionName.Split('|');
        string name = arr[0];
        switch(name)
        {
            case AnimationName1.Attack:
                StartCoroutine(delayBullet1());
                break;
            case AnimationName1.Magic:
                StartCoroutine(delayBullet2());
                break;
            case AnimationName1.Magic2:

                break;
            case AnimationName1.Ultimate:
                if (damageEffect3 != null)
                {
                    GameObject obj1 = GameObject.Instantiate(damageEffect3);
                    ParticlesEffect1 effect = obj1.AddComponent<ParticlesEffect1>();
                    Transform target = GameObject.Find("bigzhangjiao (1)").transform;
                    effect.transform.position = MathUtil1.findChild(target, "attackedPivot").position;
                    effect.play();
                }
                c.attacked();
                StartCoroutine(delayBullet3());
                StartCoroutine(delayBullet());
                break;
        }
    }
}
