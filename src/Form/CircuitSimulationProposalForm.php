<?php

/**
 * @file
 * Contains \Drupal\circuit_simulation\Form\CircuitSimulationProposalForm.
 */

namespace Drupal\circuit_simulation\Form;

use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Render\Element;
use Drupal\Core\Url;
use Drupal\Core\Link;
use Symfony\Component\HttpFoundation\RedirectResponse;
use Drupal\Core\Messenger\MessengerInterface;
use Drupal\user\Entity\User;
use Drupal\Core\Database\Database;
use Drupal\Core\Mail\MailManagerInterface;
use Drupal\Core\Render\Markup;
use Drupal\Core\StringTranslation\StringTranslationTrait;

class CircuitSimulationProposalForm extends FormBase {

  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'circuit_simulation_proposal_form';
  }

  public function buildForm(array $form, \Drupal\Core\Form\FormStateInterface $form_state, $no_js_use = NULL) {
    $user = \Drupal::currentUser();
    /************************ start approve book details ************************/
    if ($user->isAnonymous()) {
  // Create the error message with a link to the login page
  $msg = \Drupal::messenger()->addError(t('It is mandatory to ' . 
    \Drupal\Core\Link::fromTextAndUrl('login', \Drupal\Core\Url::fromRoute('user.page'))->toString() . 
    ' on this website to access the flowsheet proposal form. If you are a new user, please create a new account first.')
  );

  // Redirect to the login page
    $response = new RedirectResponse(Url::fromRoute('user.page')->toString());

  $response->send();
  
  // Return the error message (optional)
  return $msg;
}
    $query = \Drupal::database()->select('esim_circuit_simulation_proposal');
    $query->fields('esim_circuit_simulation_proposal');
    $query->condition('uid', $user->id());
    $query->orderBy('id', 'DESC');
    $query->range(0, 1);
    $proposal_q = $query->execute();
    $proposal_data = $proposal_q->fetchObject();
    if ($proposal_data) {
      if ($proposal_data->approval_status == 0 || $proposal_data->approval_status == 1) {
        $msg = \Drupal::messenger()->addError(t('We have already received your proposal.'));
          // Create a redirect response to the front page
  $response = new RedirectResponse(Url::fromRoute('<front>')->toString());
  
  // Send the redirect response
  $response->send();

        return $msg;
      } //$proposal_data->approval_status == 0 || $proposal_data->approval_status == 1
    } //$proposal_data
    $form['#attributes'] = [
      'enctype' => "multipart/form-data"
      ];
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
    ];
    $form['contributor_name'] = [
      '#type' => 'textfield',
      '#title' => t('Name of the contributor'),
      '#size' => 250,
      '#attributes' => [
        'placeholder' => t('Enter your full name.....')
        ],
      '#maxlength' => 250,
      '#required' => TRUE,
    ];
    $form['contributor_contact_no'] = [
      '#type' => 'textfield',
      '#title' => t('Contact No.'),
      '#size' => 10,
      '#attributes' => [
        'placeholder' => t('Enter your contact number')
        ],
      '#maxlength' => 250,
    ];
    $form['contributor_email_id'] = [
      '#type' => 'textfield',
      '#title' => t('Email'),
      '#size' => 30,
      '#value' => $user->getEmail(),
      '#disabled' => TRUE,
    ];
    $form['project_guide_name'] = [
      '#type' => 'textfield',
      '#title' => t('Project guide'),
      '#size' => 250,
      '#attributes' => [
        'placeholder' => t('Enter full name of project guide')
        ],
      '#maxlength' => 250,
    ];
    $form['project_guide_email_id'] = [
      '#type' => 'textfield',
      '#title' => t('Project guide email'),
      '#size' => 30,
    ];
    $form['university'] = [
      '#type' => 'textfield',
      '#title' => t('University/ Institute'),
      '#size' => 80,
      '#maxlength' => 200,
      '#required' => TRUE,
      '#attributes' => [
        'placeholder' => 'Insert full name of your institute/ university.... '
        ],
    ];
    $form['country'] = [
      '#type' => 'select',
      '#title' => t('Country'),
      '#options' => [
        'India' => 'India',
        'Others' => 'Others',
      ],
      '#required' => TRUE,
      '#tree' => TRUE,
      '#validated' => TRUE,
    ];
    $form['other_country'] = [
      '#type' => 'textfield',
      '#title' => t('Other than India'),
      '#size' => 100,
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
      '#options' => _esim_cs_list_of_states(),
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
      '#options' => _esim_cs_list_of_cities(),
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
      '#size' => 6,
    ];
    /***************************************************************************/
    $form['hr'] = [
      '#type' => 'item',
      '#markup' => '<hr>',
    ];
    $form['project_title'] = [
      '#type' => 'textfield',
      '#title' => t('Project Title'),
      '#maxlength' => 250,
      '#size' => 250,
      '#description' => t('Maximum character limit is 250'),
      '#required' => TRUE,
    ];
    $form['description'] = [
      '#type' => 'textarea',
      '#title' => t('Description'),
      '#size' => 250,
      '#description' => t('Minimum character limit is 500 and Maximum character limit is 700'),
      '#required' => TRUE,
    ];
    $form['operating_system'] = [
      '#type' => 'select',
      '#title' => t('Operating System'),
      '#options' => [
        'Ubuntu' => 'Ubuntu',
        'Windows' => 'Windows',
      ],
      '#required' => TRUE,
      '#tree' => TRUE,
      //'#validated' => TRUE
    ];
    $form['samplefile'] = [
      '#type' => 'fieldset',
      '#title' => t('Relevant Documents (if any)<span style="color:red">*</span>'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
    ];
    $form['samplefile']['samplefilepath'] = [
  '#type' => 'file',
  '#title' => $this->t('Upload circuit diagram'),
  '#description' => $this->t('Upload filenames with allowed extensions only. No spaces or any special characters allowed in the filename.') . '<br />' . '<span style="color:red;">' . $this->t('Allowed file extensions: ') . \Drupal::config('circuit_simulation.settings')->get('resource_upload_extensions') . '</span>',
  '#size' => 48,
];

    $form['reference'] = [
      '#type' => 'textfield',
      '#description' => t('The links to the documents or websites which are referenced while proposing this project.'),
      '#title' => t('Reference'),
      '#size' => 250,
      '#required' => TRUE,
      '#attributes' => [
        'placeholder' => 'Enter reference'
        ],
    ];
    $form['term_condition'] = [
      '#type' => 'checkboxes',
      '#title' => t('Terms And Conditions'),
      '#options' => [
        'status' => t('<a href="/term-and-conditions" target="_blank">I agree to the Terms and Conditions</a>')
        ],
      '#required' => TRUE,
    ];
    $form['submit'] = [
      '#type' => 'submit',
      '#value' => t('Submit'),
    ];
    return $form;
  }

  public function validateForm(array &$form, \Drupal\Core\Form\FormStateInterface $form_state) {
    if ($form_state->getValue(['term_condition']) == '1') {
      $form_state->setErrorByName('term_condition', t('Please check the terms and conditions'));
      // $form_state['values']['country'] = $form_state['values']['other_country'];
    } //$form_state['values']['term_condition'] == '1'
    if ($form_state->getValue([
      'country'
      ]) == 'Others') {
      if ($form_state->getValue(['other_country']) == '') {
        $form_state->setErrorByName('other_country', t('Enter country name'));
        // $form_state['values']['country'] = $form_state['values']['other_country'];
      } //$form_state['values']['other_country'] == ''
      else {
        $form_state->setValue(['country'], $form_state->getValue([
          'other_country'
          ]));
      }
      if ($form_state->getValue(['other_state']) == '') {
        $form_state->setErrorByName('other_state', t('Enter state name'));
        // $form_state['values']['country'] = $form_state['values']['other_country'];
      } //$form_state['values']['other_state'] == ''
      else {
        $form_state->setValue(['all_state'], $form_state->getValue([
          'other_state'
          ]));
      }
      if ($form_state->getValue(['other_city']) == '') {
        $form_state->setErrorByName('other_city', t('Enter city name'));
        // $form_state['values']['country'] = $form_state['values']['other_country'];
      } //$form_state['values']['other_city'] == ''
      else {
        $form_state->setValue(['city'], $form_state->getValue(['other_city']));
      }
    } //$form_state['values']['country'] == 'Others'
    else {
      if ($form_state->getValue(['country']) == '') {
        $form_state->setErrorByName('country', t('Select country name'));
        // $form_state['values']['country'] = $form_state['values']['other_country'];
      } //$form_state['values']['country'] == ''
      if ($form_state->getValue([
        'all_state'
        ]) == '') {
        $form_state->setErrorByName('all_state', t('Select state name'));
        // $form_state['values']['country'] = $form_state['values']['other_country'];
      } //$form_state['values']['all_state'] == ''
      if ($form_state->getValue([
        'city'
        ]) == '') {
        $form_state->setErrorByName('city', t('Select city name'));
        // $form_state['values']['country'] = $form_state['values']['other_country'];
      } //$form_state['values']['city'] == ''
    }
    //Validation for project title
    $form_state->setValue(['project_title'], trim($form_state->getValue([
      'project_title'
      ])));
    if ($form_state->getValue(['project_title']) != '') {
      if (strlen($form_state->getValue(['project_title'])) < 10) {
        $form_state->setErrorByName('project_title', t('Minimum charater limit is 10 charaters, please check the length of the project title'));
      } //strlen($form_state['values']['project_title']) < 10
    } //$form_state['values']['project_title'] != ''
    else {
      $form_state->setErrorByName('project_title', t('Project title shoud not be empty'));
    }
    $form_state->setValue(['description'], trim($form_state->getValue([
      'description'
      ])));
    if ($form_state->getValue(['description']) != '') {
      if (strlen($form_state->getValue(['description'])) > 700) {
        $form_state->setErrorByName('description', t('Maximum charater limit is 700 charaters only, please check the length of the description'));
      } //strlen($form_state['values']['project_title']) > 250
      else {
        if (strlen($form_state->getValue(['description'])) < 500) {
          $form_state->setErrorByName('description', t('Minimum charater limit is 500 charaters, please check the length of the description'));
        }
      } //strlen($form_state['values']['project_title']) < 10
    } //$form_state['values']['project_title'] != ''
    else {
      $form_state->setErrorByName('description', t('Description shoud not be empty'));
    }
    if (isset($_FILES['files'])) {
      /* check if atleast one source or result file is uploaded */
      if (!($_FILES['files']['name']['samplefilepath'])) {
        $form_state->setErrorByName('samplefilepath', t('Please upload file with circuit diagram.'));
      }
      /* check for valid filename extensions */
      foreach ($_FILES['files']['name'] as $file_form_name => $file_name) {
        if ($file_name) {
          $allowed_extensions_str = \Drupal::config('circuit_simulation.settings')->get('resource_upload_extensions');
          $allowed_extensions = explode(',', $allowed_extensions_str);
          $fnames = explode('.', strtolower($_FILES['files']['name'][$file_form_name]));
          $temp_extension = end($fnames);
          if (!in_array($temp_extension, $allowed_extensions)) {
            $form_state->setErrorByName($file_form_name, t('Only file with ' . $allowed_extensions_str . ' extensions can be uploaded.'));
          }
          if ($_FILES['files']['size'][$file_form_name] <= 0) {
            $form_state->setErrorByName($file_form_name, t('File size cannot be zero.'));
          }
          /* check if valid file name */
          if (!circuit_simulation_check_valid_filename($_FILES['files']['name'][$file_form_name])) {
            $form_state->setErrorByName($file_form_name, t('Invalid file name specified. Only alphabets and numbers are allowed as a valid filename.'));
          }
        } //$file_name
      } //$_FILES['files']['name'] as $file_form_name => $file_name
    }
    return $form_state;
  }

  public function submitForm(array &$form, \Drupal\Core\Form\FormStateInterface $form_state) {
    $user = \Drupal::currentUser();
    $root_path = circuit_simulation_path();
    if (!$user->id()) {
      \Drupal::messenger()->addError('It is mandatory to login on this website to access the proposal form');
      return;
    } //!$user->uid
	/*if ($form_state['values']['version'] == 'Old version')
	{
		$form_state['values']['version'] = trim($form_state['values']['older']);
	} *///$form_state['values']['version'] == 'Old version'
	/* inserting the user proposal */
    $v = $form_state->getValues();
    $project_title = trim($v['project_title']);
    $proposar_name = $v['name_title'] . ' ' . $v['contributor_name'];
    $university = $v['university'];
    $directory_name = _cs_dir_name($project_title, $proposar_name);
    $result = "INSERT INTO {esim_circuit_simulation_proposal} 
    (
    uid, 
    approver_uid,
    name_title, 
    contributor_name,
    contact_no,
    university,
    city, 
    pincode, 
    state, 
    country,
    project_guide_name,
    project_guide_email_id,
    project_title, 
    description,
    operating_system,
    directory_name,
    approval_status,
    is_completed, 
    dissapproval_reason,
    creation_date, 
    approval_date,
    samplefilepath,
    reference
    ) VALUES
    (
    :uid, 
    :approver_uid, 
    :name_title, 
    :contributor_name, 
    :contact_no,
    :university, 
    :city, 
    :pincode, 
    :state,  
    :country,
    :project_guide_name,
    :project_guide_email_id,
    :project_title, 
    :description,
    :operating_system,
    :directory_name,
    :approval_status,
    :is_completed, 
    :dissapproval_reason,
    :creation_date, 
    :approval_date,
    :samplefilepath,
    :reference
    )";
    $args = [
      ":uid" => $user->id(),
      ":approver_uid" => 0,
      ":name_title" => $v['name_title'],
      ":contributor_name" => _esim_cs_sentence_case(trim($v['contributor_name'])),
      ":contact_no" => $v['contributor_contact_no'],
      ":university" => _esim_cs_sentence_case($v['university']),
      ":city" => $v['city'],
      ":pincode" => $v['pincode'],
      ":state" => $v['all_state'],
      ":country" => $v['country'],
      ":project_guide_name" => _esim_cs_sentence_case($v['project_guide_name']),
      ":project_guide_email_id" => trim($v['project_guide_email_id']),
      ":project_title" => _esim_cs_sentence_case($v['project_title']),
      ":description" => _esim_cs_sentence_case($v['description']),
      ":operating_system" => $v['operating_system'],
      ":directory_name" => $directory_name,
      ":approval_status" => 0,
      ":is_completed" => 0,
      ":dissapproval_reason" => "NULL",
      ":creation_date" => time(),
      ":approval_date" => 0,
      ":samplefilepath" => "",
      ":reference" => $v['reference'],
    ];
    //	var_dump($args);die;
    //var_dump($result);die;
    $connection = Database::getConnection();
$proposal_id= $connection->insert('esim_circuit_simulation_proposal')->fields($args)->execute();

    $dest_path = $directory_name . '/';
    $dest_path1 = $root_path . $dest_path;
    //var_dump($dest_path1);die;	
    if (!is_dir($root_path . $dest_path)) {
      mkdir($root_path . $dest_path);
    }
    /* uploading files */
    foreach ($_FILES['files']['name'] as $file_form_name => $file_name) {
      if ($file_name) {
        /* checking file type */
        //$file_type = 'S';
        if (file_exists($root_path . $dest_path . $_FILES['files']['name'][$file_form_name])) {
          \Drupal::messenger()->addError(t("Error uploading file. File !filename already exists.", [
            '!filename' => $_FILES['files']['name'][$file_form_name]
            ]));
          //unlink($root_path . $dest_path . $_FILES['files']['name'][$file_form_name]);
        } //file_exists($root_path . $dest_path . $_FILES['files']['name'][$file_form_name])
			/* uploading file */
        if (move_uploaded_file($_FILES['files']['tmp_name'][$file_form_name], $root_path . $dest_path . $_FILES['files']['name'][$file_form_name])) {
          $query = "UPDATE esim_circuit_simulation_proposal SET samplefilepath = :samplefilepath WHERE id = :id";
          $args = [
            ":samplefilepath" => $dest_path . $_FILES['files']['name'][$file_form_name],
            ":id" => $proposal_id,
          ];

          $updateresult = \Drupal::database()->query($query, $args);
          //var_dump($args);die;

          \Drupal::messenger()->addStatus($file_name . ' uploaded successfully.');
        } //move_uploaded_file($_FILES['files']['tmp_name'][$file_form_name], $root_path . $dest_path . $_FILES['files']['name'][$file_form_name])
        else {
          \Drupal::messenger()->addError('Error uploading file : ' . $dest_path . '/' . $file_name);
        }
      } //$file_name
    } //$_FILES['files']['name'] as $file_form_name => $file_name
    if (!$proposal_id) {
      \Drupal::messenger()->addError(t('Error receiving your proposal. Please try again.'));
      return;
    } //!$proposal_id
	/* sending email */
    $email_to = $user->getEmail();
    $form = \Drupal::config('circuit_simulation.settings')->get('circuit_simulation_from_email');
    $bcc = \Drupal::config('circuit_simulation.settings')->get('circuit_simulation_emails');
    $cc = \Drupal::config('circuit_simulation.settings')->get('circuit_simulation_cc_emails');
    $params['circuit_simulation_proposal_received']['proposal_id'] = $proposal_id;
    $params['circuit_simulation_proposal_received']['user_id'] = $user->id();
    $params['circuit_simulation_proposal_received']['headers'] = [
      'From' => $form,
      'MIME-Version' => '1.0',
      'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
      'Content-Transfer-Encoding' => '8Bit',
      'X-Mailer' => 'Drupal',
      'Cc' => $cc,
      'Bcc' => $bcc,
    ];
    //\Drupal::service('plugin.manager.mail')->mail('circuit_simulation', 'circuit_simulation_proposal_received', $email_to, 'en', $params, $form, TRUE);
    if (!\Drupal::service('plugin.manager.mail')->mail('circuit_simulation', 'circuit_simulation_proposal_received', $email_to, 'en', $params, $form, TRUE)) {
      \Drupal::messenger()->addError('Error sending email message.');
    }
    \Drupal::messenger()->addStatus(t('We have received your eSim circuit simulation proposal. We will get back to you soon.'));
    $response = new RedirectResponse(Url::fromRoute('<front>')->toString());
    // Send the redirect response
    $response->send();
  }

}
?>
