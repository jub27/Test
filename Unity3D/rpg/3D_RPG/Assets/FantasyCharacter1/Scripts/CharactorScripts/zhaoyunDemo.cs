using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class caocaoDemo : MonoBehaviour {

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
        int count = 5;
        AttackedController c = GameObject.Find("bigzhangjiao (1)").GetComponent<AttackedController>();
        for (int i = 0; i < count; i++)
        {
            GameObject obj = GameObject.Instantiate(attackBullet);
            PosBullet bullet = obj.GetComponent<PosBullet>();
            bullet.player = transform;
            Vector3 newPos = c.transform.position + new Vector3(Random.Range(-5f, 5f), 0f, Random.Range(-5f, 5f));
            Vector3 attackedPos = MathUtil1.findChild(c.transform, "attackedPivot").position;
            bullet.startPos = MathUtil1.calcTargetPosByRotation(attackedPos, Quaternion.LookRotation(newPos - c.transform.position), 0f, 5f);
            bullet.tarPos = attackedPos - bullet.startPos + attackedPos;
            bullet.effectObj = damageEffect1;
            bullet.bulleting();
            yield return new WaitForSeconds(0.05f);
            if (i % 3 == 0)
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

    IEnumerator delayBullet1()
    {
        int count = 20;
        AttackedController c = GameObject.Find("bigzhangjiao (1)").GetComponent<AttackedController>();
        for (int i = 0; i < count; i++)
        {
            GameObject obj = GameObject.Instantiate(attackBullet);
            PosBullet bullet = obj.GetComponent<PosBullet>();
            bullet.player = transform;
            Vector3 newPos = c.transform.position + new Vector3(Random.Range(-1f, 1f), Random.Range(5f, 10f), Random.Range(-1f, 1f));
            Vector3 attackedPos = MathUtil1.findChild(c.transform, "attackedPivot").position;
            bullet.startPos = MathUtil1.calcTargetPosByRotation(attackedPos, Quaternion.LookRotation(newPos - c.transform.position), 0f, 5f);
            bullet.tarPos = bullet.startPos - new Vector3(0f, 10f, 0f);
            bullet.effectObj = damageEffect1;
            bullet.bulleting();
            yield return new WaitForSeconds(0.05f);
            if (i % 3 == 0)
            {
                c.attacked();
                
            }
        }
    }

    void preAction(string actionName)
    {
        AttackedController c = GameObject.Find("bigzhangjiao (1)").GetComponent<AttackedController>();
        string[] arr = actionName.Split('|');
        string name = arr[0];
        switch(name)
        {
            case AnimationName1.Attack:
                if (damageEffect1 != null)
                {
                    GameObject obj = GameObject.Instantiate(damageEffect1);
                    ParticlesEffect1 effect = obj.AddComponent<ParticlesEffect1>();
                    Transform target = GameObject.Find("bigzhangjiao (1)").transform;
                    effect.transform.position = MathUtil1.findChild(target, "attackedPivot").position;
                    effect.play();
                }
                c.attacked();
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
                    Transform target = GameObject.Find("bigzhangjiao (1)").transform;
                    effect.transform.position = MathUtil1.findChild(target, "attackedPivot").position;
                    effect.play();
                }
                c.attacked();
                //StartCoroutine(delayBullet());
                StartCoroutine(delayBullet1());
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
                if (damageEffect3 != null)
                {
                    GameObject obj = GameObject.Instantiate(damageEffect3);
                    ParticlesEffect1 effect = obj.AddComponent<ParticlesEffect1>();

                    effect.transform.position = GameObject.Find("bigzhangjiao (1)").transform.position;
                    effect.play();
                }
                c.attacked();
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
                }
                c.attacked();
                //StartCoroutine(delayBullet());
                StartCoroutine(delayBullet1());
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
