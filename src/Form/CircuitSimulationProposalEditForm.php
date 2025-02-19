<?php

/**
 * @file
 * Contains \Drupal\circuit_simulation\Form\CircuitSimulationProposalEditForm.
 */

namespace Drupal\circuit_simulation\Form;

use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Render\Element;
use Drupal\Core\Link;
use Drupal\Core\Url;
use Symfony\Component\HttpFoundation\RedirectResponse;
use Drupal\Core\Messenger\MessengerInterface;
use Drupal\user\Entity\User;
use Drupal\Core\Database\Database;

class CircuitSimulationProposalEditForm extends FormBase {

  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'circuit_simulation_proposal_edit_form';
  }

  public function buildForm(array $form, \Drupal\Core\Form\FormStateInterface $form_state) {
    $user = \Drupal::currentUser();
    $service = \Drupal::service('circuit_simulation_global');
    /* get current proposal */
    $proposal_id = \Drupal::routeMatch()->getParameter('proposal_id');
    //$proposal_q = db_query("SELECT * FROM {esim_circuit_simulation_proposal} WHERE id = %d", $proposal_id);
    $query = \Drupal::database()->select('esim_circuit_simulation_proposal');
    $query->fields('esim_circuit_simulation_proposal');
    $query->condition('id', $proposal_id);
    $proposal_q = $query->execute();
    if ($proposal_q) {
      if ($proposal_data = $proposal_q->fetchObject()) {
        /* everything ok */
      } //$proposal_data = $proposal_q->fetchObject()
      else {
        $msg = \Drupal::messenger()->addError(t('Invalid proposal selected. Please try again.'));
        $response = new RedirectResponse(Url::fromRoute('circuit_simulation.proposal_pending')->toString());
         $response->send();
        //drupal_goto('circuit-simulation-project/manage-proposal');
        return $msg;
      }
    } //$proposal_q
    else {
      $msg = \Drupal::messenger()->addError(t('Invalid proposal selected. Please try again.'));
      $response = new RedirectResponse(Url::fromRoute('circuit_simulation.proposal_pending')->toString());
       $response->send();
      return $msg;
    }
    $user_data = \Drupal::entityTypeManager()->getStorage('user')->load($proposal_data->uid);
    $form['name_title'] = [
      '#type' => 'select',
      '#title' => t('Title'),
      '#options' => [
        'Dr' => 'Dr',
        'Prof' => 'Prof',
        'Mr' => 'Mr',
        'Mrs' => 'Mrs',
        'Ms' => 'Ms',
      ],
      '#required' => TRUE,
      '#default_value' => $proposal_data->name_title,
    ];
    $form['contributor_name'] = [
      '#type' => 'textfield',
      '#title' => t('Name of the Proposer'),
      '#size' => 30,
      '#maxlength' => 50,
      '#required' => TRUE,
      '#default_value' => $proposal_data->contributor_name,
    ];
    $form['student_email_id'] = [
      '#type' => 'item',
      '#title' => t('Email'),
      '#markup' => $user_data->getEmail(),
    ];
    $form['university'] = [
      '#type' => 'textfield',
      '#title' => t('University/Institute'),
      '#size' => 200,
      '#maxlength' => 200,
      '#required' => TRUE,
      '#default_value' => $proposal_data->university,
    ];
    $form['project_guide_name'] = [
      '#type' => 'textfield',
      '#title' => t('Project guide'),
      '#size' => 250,
      '#attributes' => [
        'placeholder' => t('Enter full name of project guide')
        ],
      '#maxlength' => 250,
      '#default_value' => $proposal_data->project_guide_name,
    ];
    $form['project_guide_email_id'] = [
      '#type' => 'textfield',
      '#title' => t('Project guide email'),
      '#size' => 30,
      '#default_value' => $proposal_data->project_guide_email_id,
    ];
    $form['country'] = [
      '#type' => 'select',
      '#title' => t('Country'),
      '#options' => [
        'India' => 'India',
        'Others' => 'Others',
      ],
      '#default_value' => $proposal_data->country,
      '#required' => TRUE,
      '#tree' => TRUE,
      '#validated' => TRUE,
    ];
    $form['other_country'] = [
      '#type' => 'textfield',
      '#title' => t('Other than India'),
      '#size' => 100,
      '#default_value' => $proposal_data->country,
      '#attributes' => [
        'placeholder' => t('Enter your country name')
        ],
      '#states' => [
        'visible' => [
          ':input[name="country"]' => [
            'value' => 'Others'
            ]
          ]
        ],
    ];
    $form['other_state'] = [
      '#type' => 'textfield',
      '#title' => t('State other than India'),
      '#size' => 100,
      '#attributes' => [
        'placeholder' => t('Enter your state/region name')
        ],
      '#default_value' => $proposal_data->state,
      '#states' => [
        'visible' => [
          ':input[name="country"]' => [
            'value' => 'Others'
            ]
          ]
        ],
    ];
    $form['other_city'] = [
      '#type' => 'textfield',
      '#title' => t('City other than India'),
      '#size' => 100,
      '#attributes' => [
        'placeholder' => t('Enter your city name')
        ],
      '#default_value' => $proposal_data->city,
      '#states' => [
        'visible' => [
          ':input[name="country"]' => [
            'value' => 'Others'
            ]
          ]
        ],
    ];
    $form['all_state'] = [
      '#type' => 'select',
      '#title' => t('State'),
      '#options' => $service->_esim_cs_list_of_states(),
      '#default_value' => $proposal_data->state,
      '#validated' => TRUE,
      '#states' => [
        'visible' => [
          ':input[name="country"]' => [
            'value' => 'India'
            ]
          ]
        ],
    ];
    $form['city'] = [
      '#type' => 'select',
      '#title' => t('City'),
      '#options' => $service->_esim_cs_list_of_cities(),
      '#default_value' => $proposal_data->city,
      '#states' => [
        'visible' => [
          ':input[name="country"]' => [
            'value' => 'India'
            ]
          ]
        ],
    ];
    $form['pincode'] = [
      '#type' => 'textfield',
      '#title' => t('Pincode'),
      '#size' => 30,
      '#maxlength' => 6,
      '#default_value' => $proposal_data->pincode,
      '#attributes' => [
        'placeholder' => 'Insert pincode of your city/ village....'
        ],
    ];
    $form['operating_system'] = [
      '#type' => 'select',
      '#title' => t('Operating System'),
      '#options' => [
        'Ubuntu' => 'Ubuntu',
        'Windows' => 'Windows',
      ],
      '#required' => TRUE,
      '#default_value' => $proposal_data->operating_system,
    ];
    $form['project_title'] = [
      '#type' => 'textfield',
      '#title' => t('Title of the Circuit Simulation Project'),
      '#size' => 300,
      '#maxlength' => 350,
      '#required' => TRUE,
      '#default_value' => $proposal_data->project_title,
    ];
    $form['description'] = [
      '#type' => 'textarea',
      '#title' => t('Description'),
      '#description' => t('Minimum character limit is 500 and Maximum character limit is 700'),
      '#required' => TRUE,
      '#default_value' => $proposal_data->description,
    ];
    $form['reference'] = [
      '#type' => 'textarea',
      '#title' => t('Reference'),
      '#size' => 250,
      '#maxlength' => 1000,
      '#attributes' => [
        'placeholder' => 'Links of must be provided....'
        ],
      '#default_value' => $proposal_data->reference,
    ];
    $form['delete_proposal'] = [
      '#type' => 'checkbox',
      '#title' => t('Delete Proposal'),
    ];
    $form['submit'] = [
      '#type' => 'submit',
      '#value' => t('Submit'),
    ];
    $cancel_url = Url::fromUri('internal:/circuit-simulation-project/manage-proposal');

    $form['cancel'] = array(
    		'#type' => 'item',
    		'#markup' => Link::fromTextAndUrl('Cancel', $cancel_url)->toString()
    	);

    return $form;
  }

  public function submitForm(array &$form, \Drupal\Core\Form\FormStateInterface $form_state) {
    $user = \Drupal::currentUser();
    $service = \Drupal::service('circuit_simulation_global');
    /* get current proposal */
    $proposal_id = \Drupal::routeMatch()->getParameter('proposal_id');
    $query = \Drupal::database()->select('esim_circuit_simulation_proposal');
    $query->fields('esim_circuit_simulation_proposal');
    $query->condition('id', $proposal_id);
    $proposal_q = $query->execute();
    if ($proposal_q) {
      if ($proposal_data = $proposal_q->fetchObject()) {
        /* everything ok */
      } //$proposal_data = $proposal_q->fetchObject()
      else {
        $msg = \Drupal::messenger()->addError(t('Invalid proposal selected. Please try again.'));
        $response = new RedirectResponse(Url::fromRoute('circuit_simulation.proposal_pending')->toString());
         $response->send();
        //drupal_goto('circuit-simulation-project/manage-proposal');
        return $msg;
      }
    } //$proposal_q
    else {
      $msg = \Drupal::messenger()->addError(t('Invalid proposal selected. Please try again.'));
      $response = new RedirectResponse(Url::fromRoute('circuit_simulation.proposal_pending')->toString());
       $response->send();
      return $msg;
    }
    /* delete proposal */
    if ($form_state->getValue(['delete_proposal']) == 1) {
      /* sending email */
      $user_data = \Drupal::entityTypeManager()->getStorage('user')->load($proposal_data->uid);
      // $email_to = $user_data->getEmail();
      // $from = \Drupal::config('circuit_simulation.settings')->get('circuit_simulation_from_email');
      // $bcc = \Drupal::config('circuit_simulation.settings')->get('circuit_simulation_emails');
      // $cc = \Drupal::config('circuit_simulation.settings')->get('circuit_simulation_cc_emails');
      // $params['circuit_simulation_proposal_deleted']['proposal_id'] = $proposal_id;
      // $params['circuit_simulation_proposal_deleted']['user_id'] = $proposal_data->uid;
      // $params['circuit_simulation_proposal_deleted']['headers'] = [
      //   'From' => $from,
      //   'MIME-Version' => '1.0',
      //   'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
      //   'Content-Transfer-Encoding' => '8Bit',
      //   'X-Mailer' => 'Drupal',
      //   'Cc' => $cc,
      //   'Bcc' => $bcc,
      // ];
      // if (!drupal_mail('circuit_simulation', 'circuit_simulation_proposal_deleted', $email_to, user_preferred_language($user), $params, $from, TRUE)) {
      //   \Drupal::messenger()->addError('Error sending email message.');
      // }
      \Drupal::messenger()->addStatus(t('eSim Cicuit simulation proposal has been deleted.'));
      if ($service->rrmdir_project($proposal_id) == TRUE) {
        $query = \Drupal::database()->delete('esim_circuit_simulation_proposal');
        $query->condition('id', $proposal_id);
        $num_deleted = $query->execute();
        $msg = \Drupal::messenger()->addStatus(t('Proposal Deleted'));
        $response = new RedirectResponse(Url::fromRoute('circuit_simulation.proposal_all')->toString());
         $response->send();
        return $msg;
      } //rrmdir_project($proposal_id) == TRUE
    } //$form_state['values']['delete_proposal'] == 1
	/* update proposal */
    $v = $form_state->getValues();
    $project_title = $v['project_title'];
    $proposar_name = $v['name_title'] . ' ' . $v['contributor_name'];
    $university = $v['university'];
    $directory_names = $service->_cs_dir_name($project_title, $proposar_name);
    if ($service->CS_RenameDir($proposal_id, $directory_names)) {
      $directory_name = $directory_names;
    } //LM_RenameDir($proposal_id, $directory_names)
    else {
      return;
    }
    $str = substr($proposal_data->samplefilepath, strrpos($proposal_data->samplefilepath, '/'));
    $resource_file = ltrim($str, '/');
    $samplefilepath = $directory_name . '/' . $resource_file;
    $query = "UPDATE esim_circuit_simulation_proposal SET 
				name_title=:name_title,
				contributor_name=:contributor_name,
				university=:university,
				project_guide_name=:project_guide_name,
				project_guide_email_id=:project_guide_email_id,
				city=:city,
				pincode=:pincode,
				state=:state,
				project_title=:project_title,
				description=:description,
				operating_system=:operating_system,
				reference=:reference,
				directory_name=:directory_name ,
				samplefilepath=:samplefilepath
				WHERE id=:proposal_id";
    $args = [
      ':name_title' => $v['name_title'],
      ':contributor_name' => $v['contributor_name'],
      ':university' => $v['university'],
      ':project_guide_name' => $v['project_guide_name'],
      ':project_guide_email_id' => $v['project_guide_email_id'],
      ':city' => $v['city'],
      ':pincode' => $v['pincode'],
      ':state' => $v['all_state'],
      ':project_title' => $project_title,
      ':description' => $v['description'],
      ':operating_system' => $v['operating_system'],
      ':reference' => $v['reference'],
      ':directory_name' => $directory_name,
      ':samplefilepath' => $samplefilepath,
      ':proposal_id' => $proposal_id,
    ];
    $result = \Drupal::database()->query($query, $args);
    \Drupal::messenger()->addStatus(t('Proposal Updated'));
  }

}
?>
