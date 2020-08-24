using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class huanggaiDemo : MonoBehaviour {

    public GameObject attackBullet;
    public GameObject magicBullet;
    public GameObject magic2Bullet;
    public GameObject ultimateBullet;
    public GameObject damageEffect1;
    public GameObject damageEffect2;
    public GameObject damageEffect3;
    public GameObject damageEffect4;
	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		
	}


    IEnumerator delayBullet()
    {
        int count = 2;
        for (int i = 0; i < count; i++)
        {
            AttackedController c = GameObject.Find("bigzhangjiao (1)").GetComponent<AttackedController>();
            GameObject obj = GameObject.Instantiate(attackBullet);
            CurvelBullet bullet = obj.GetComponent<CurvelBullet>();
            bullet.player = transform;
            bullet.target = c.transform;

            bullet.bulleting();
            yield return new WaitForSeconds(0.5f);
            if (i % 9 == 0)
            {
                bullet.effectObj = damageEffect4;
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
        int count = 4;
        for (int i = 0; i < count; i++)
        {
            AttackedController c = GameObject.Find("bigzhangjiao (1)").GetComponent<AttackedController>();
            GameObject obj = GameObject.Instantiate(attackBullet);
            CurvelBullet bullet = obj.GetComponent<CurvelBullet>();
            bullet.player = transform;
            bullet.target = c.transform;

            bullet.bulleting();
            yield return new WaitForSeconds(0.5f);
            if (i % 9 == 0)
            {
                bullet.effectObj = damageEffect4;
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
        int count = 8;
        for (int i = 0; i < count; i++)
        {
            AttackedController c = GameObject.Find("bigzhangjiao (1)").GetComponent<AttackedController>();
            GameObject obj = GameObject.Instantiate(attackBullet);
            CurvelBullet bullet = obj.GetComponent<CurvelBullet>();
            bullet.player = transform;
            bullet.target = c.transform;
            bullet.effectObj = damageEffect4;
            bullet.bulleting();
            yield return new WaitForSeconds(0.5f);
            if (i % 9 == 0)
            {
               
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
        string[] arr = actionName.Split('|');
        string name = arr[0];
        switch(name)
        {
            case AnimationName1.Attack:
                if (damageEffect1 != null)
                {
                    GameObject obj = GameObject.Instantiate(damageEffect1);
                    ParticlesEffect1 effect = obj.AddComponent<ParticlesEffect1>();

                    effect.transform.position = GameObject.Find("bigzhangjiao (1)").transform.position;
                    effect.play();
                    StartCoroutine(delayAttacked());
                }
                StartCoroutine(delayBullet());
                break;
            case AnimationName1.Magic:
                if (magicBullet != null)
                {
                    GameObject obj = GameObject.Instantiate(magicBullet);
                    NormalBullet bullet = obj.GetComponent<NormalBullet>();
                    bullet.player = transform;
                    bullet.target = GameObject.Find("bigzhangjiao (1)").transform;
                    bullet.effectObj = damageEffect1;
                    bullet.bulleting();
                }
                if (damageEffect2 != null)
                {
                    GameObject obj = GameObject.Instantiate(damageEffect2);
                    ParticlesEffect1 effect = obj.AddComponent<ParticlesEffect1>();

                    effect.transform.position = GameObject.Find("bigzhangjiao (1)").transform.position;
                    effect.play();
                    StartCoroutine(delayAttacked());
                }
                StartCoroutine(delayBullet2());
                break;
            case AnimationName1.Magic2:
                if (magic2Bullet != null)
                {
                    GameObject obj = GameObject.Instantiate(magic2Bullet);
                    NormalBullet bullet = obj.GetComponent<NormalBullet>();
                    bullet.player = transform;
                    bullet.target = GameObject.Find("bigzhangjiao (1)").transform;
                    bullet.effectObj = damageEffect2;
                    bullet.bulleting();
                }
                if (damageEffect2 != null)
                {
                    GameObject obj = GameObject.Instantiate(damageEffect2);
                    ParticlesEffect1 effect = obj.AddComponent<ParticlesEffect1>();

                    effect.transform.position = GameObject.Find("bigzhangjiao (1)").transform.position;
                    effect.play();
                    StartCoroutine(delayAttacked());
                }
                StartCoroutine(delayBullet3());
                break;
            case AnimationName1.Ultimate:
                if (ultimateBullet != null)
                {
                    GameObject obj = GameObject.Instantiate(ultimateBullet);
                    LightBullet1 bullet = obj.GetComponent<LightBullet1>();
                    bullet.player = transform;
                    bullet.target = GameObject.Find("bigzhangjiao (1)").transform;
                    bullet.effectObj = damageEffect3;
                    bullet.bulleting();
                }
                if(damageEffect3 != null)
                {
                    GameObject obj = GameObject.Instantiate(damageEffect3);
                    ParticlesEffect1 effect = obj.AddComponent<ParticlesEffect1>();

                    effect.transform.position = GameObject.Find("bigzhangjiao (1)").transform.position;
                    effect.play();
                    StartCoroutine(delayAttacked());
                }
                StartCoroutine(delayBullet3());
                break;
        }
    }

    IEnumerator delayAttacked()
    {
        yield return new WaitForSeconds(1.5f);
        AttackedController c = GameObject.Find("bigzhangjiao (1)").GetComponent<AttackedController>();
        c.attacked();
        //yield return new WaitForSeconds(2.5f);
        //c.attacked();
    }
}
