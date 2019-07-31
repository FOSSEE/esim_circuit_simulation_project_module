<?php
// $Id$
/*
Approval Status :
0 - Pending
1 - Approved
2 - Dis-Approved
3 - Completed
Solution Status :
0 - Pending
1 - Approved
2 - Dis-Approved
Solution Display :
0 - No
1 - Yes


is_completed
0 - in progress
1 - Completed
Tables :
circuit_simulation_solution : approval_status
0 - Pending
1 - Approved
2 - Disapproved (delete it)
*/
function circuit_simulation_proposal_form($form, &$form_state, $no_js_use = FALSE)
{
	global $user;
	/************************ start approve book details ************************/
	if ($user->uid == 0)
	{
		$msg = drupal_set_message(t('It is mandatory to ' . l('login', 'user') . ' on this website to access the circuit simulation proposal form. If you are new user please create a new account first.'), 'error');
		//drupal_goto('esim-circuit-simulation-project');
		drupal_goto('user');
		return $msg;
	} //$user->uid == 0
	$query = db_select('esim_circuit_simulation_proposal');
	$query->fields('esim_circuit_simulation_proposal');
	$query->condition('uid', $user->uid);
	$query->orderBy('id', 'DESC');
	$query->range(0, 1);
	$proposal_q = $query->execute();
	$proposal_data = $proposal_q->fetchObject();
	if ($proposal_data)
	{
		if ($proposal_data->approval_status == 0 || $proposal_data->approval_status == 1)
		{
			drupal_set_message(t('We have already received your proposal.'), 'status');
			drupal_goto('');
			return;
		} //$proposal_data->approval_status == 0 || $proposal_data->approval_status == 1
	} //$proposal_data
	$form['#attributes'] = array(
		'enctype' => "multipart/form-data"
	);
	$form['name_title'] = array(
		'#type' => 'select',
		'#title' => t('Title'),
		'#options' => array(
			'Dr' => 'Dr',
			'Prof' => 'Prof',
			'Mr' => 'Mr',
			'Mrs' => 'Mrs',
			'Ms' => 'Ms'
		),
		'#required' => TRUE
	);
	$form['contributor_name'] = array(
		'#type' => 'textfield',
		'#title' => t('Name of the contributor'),
		'#size' => 250,
		'#attributes' => array(
			'placeholder' => t('Enter your full name.....')
		),
		'#maxlength' => 250,
		'#required' => TRUE
	);
	$form['contributor_contact_no'] = array(
		'#type' => 'textfield',
		'#title' => t('Contact No.'),
		'#size' => 10,
		'#attributes' => array(
			'placeholder' => t('Enter your contact number')
		),
		'#maxlength' => 250
	);
	$form['contributor_email_id'] = array(
		'#type' => 'textfield',
		'#title' => t('Email'),
		'#size' => 30,
		'#value' => $user->mail,
		'#disabled' => TRUE
	);
	$form['project_guide_name'] = array(
		'#type' => 'textfield',
		'#title' => t('Project guide'),
		'#size' => 250,
		'#attributes' => array(
			'placeholder' => t('Enter full name of project guide')
		),
		'#maxlength' => 250
	);
	$form['project_guide_email_id'] = array(
		'#type' => 'textfield',
		'#title' => t('Project guide email'),
		'#size' => 30
	);
	$form['university'] = array(
		'#type' => 'textfield',
		'#title' => t('University/ Institute'),
		'#size' => 80,
		'#maxlength' => 200,
		'#required' => TRUE,
		'#attributes' => array(
			'placeholder' => 'Insert full name of your institute/ university.... '
		)
	);
	$form['country'] = array(
		'#type' => 'select',
		'#title' => t('Country'),
		'#options' => array(
			'India' => 'India',
			'Others' => 'Others'
		),
		'#required' => TRUE,
		'#tree' => TRUE,
		'#validated' => TRUE
	);
	$form['other_country'] = array(
		'#type' => 'textfield',
		'#title' => t('Other than India'),
		'#size' => 100,
		'#attributes' => array(
			'placeholder' => t('Enter your country name')
		),
		'#states' => array(
			'visible' => array(
				':input[name="country"]' => array(
					'value' => 'Others'
				)
			)
		)
	);
	$form['other_state'] = array(
		'#type' => 'textfield',
		'#title' => t('State other than India'),
		'#size' => 100,
		'#attributes' => array(
			'placeholder' => t('Enter your state/region name')
		),
		'#states' => array(
			'visible' => array(
				':input[name="country"]' => array(
					'value' => 'Others'
				)
			)
		)
	);
	$form['other_city'] = array(
		'#type' => 'textfield',
		'#title' => t('City other than India'),
		'#size' => 100,
		'#attributes' => array(
			'placeholder' => t('Enter your city name')
		),
		'#states' => array(
			'visible' => array(
				':input[name="country"]' => array(
					'value' => 'Others'
				)
			)
		)
	);
	$form['all_state'] = array(
		'#type' => 'select',
		'#title' => t('State'),
		'#options' => _df_list_of_states(),
		'#validated' => TRUE,
		'#states' => array(
			'visible' => array(
				':input[name="country"]' => array(
					'value' => 'India'
				)
			)
		)
	);
	$form['city'] = array(
		'#type' => 'select',
		'#title' => t('City'),
		'#options' => _df_list_of_cities(),
		'#states' => array(
			'visible' => array(
				':input[name="country"]' => array(
					'value' => 'India'
				)
			)
		)
	);
	$form['pincode'] = array(
		'#type' => 'textfield',
		'#title' => t('Pincode'),
		'#size' => 6
	);
	/***************************************************************************/
	$form['hr'] = array(
		'#type' => 'item',
		'#markup' => '<hr>'
	);
	$form['project_title'] = array(
		'#type' => 'textarea',
		'#title' => t('Project Title'),
		'#size' => 250,
		'#description' => t('Maximum character limit is 250'),
		'#required' => TRUE
	);
	$form['description'] = array(
		'#type' => 'textarea',
		'#title' => t('Description'),
		'#size' => 250,
		'#description' => t('Minimum character limit is 500 and Maximum character limit is 700'),
		'#required' => TRUE
	);	
	$form['operating_system'] = array(
		'#type' => 'select',
		'#title' => t('Operating System'),
		'#options' => array(
			'Ubuntu' => 'Ubuntu',
			'Windows' => 'Windows'
		),
		'#required' => TRUE,
		'#tree' => TRUE,
		//'#validated' => TRUE
	);
	$form['samplefile'] = array(
		'#type' => 'fieldset',
		'#title' => t('Relevant Documents (if any)'),
		'#collapsible' => FALSE,
		'#collapsed' => FALSE
	);
	$form['samplefile']['samplefilepath'] = array(
		'#type' => 'file',
		//'#title' => t('Upload circuit diagram'),
		'#size' => 48,
		'#description' => t('Upload filenames with allowed extensions only. No spaces or any special characters allowed in filename.') . '<br />' . t('<span style="color:red;">Allowed file extensions : ') . variable_get('resource_upload_extensions', '') . '</span>'
	);
	$form['reference'] = array(
		'#type' => 'textfield',
		'#description' => t('The links to the documents or websites which are referenced while proposing this project.'),
		'#title' => t('Reference'),
		'#size' => 250,
		'#required' => TRUE,

		'#attributes' => array(
			'placeholder' => 'Enter reference'
		)

	);
	/*$form['fellowship'] = array(
		'#type' => 'radios',
		'#title' => t('If you are applying for FOSSEE Summer Fellowship 2018 - eSim Screening level Task 1, select Yes.(To know more about the FOSSEE Summer Fellowship 2018 <a href="https://fossee.in/fellowship/" target="_blank">Click here</a>)'),
		'#options' => array(
			'1' => 'Yes',
			'2' => 'No'
		),
		'#required' => TRUE
	);*/
	$form['term_condition'] = array(
		'#type' => 'checkboxes',
		'#title' => t('Terms And Conditions'),
		'#options' => array(
			'status' => t('<a href="/term-and-conditions" target="_blank">I agree to the Terms and Conditions</a>')
		),
		'#required' => TRUE
	);
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit')
	);
	return $form;
}
function circuit_simulation_proposal_form_validate($form, &$form_state)
{
	if ($form_state['values']['term_condition'] == '1')
	{
		form_set_error('term_condition', t('Please check the terms and conditions'));
		// $form_state['values']['country'] = $form_state['values']['other_country'];
	} //$form_state['values']['term_condition'] == '1'
	if ($form_state['values']['country'] == 'Others')
	{
		if ($form_state['values']['other_country'] == '')
		{
			form_set_error('other_country', t('Enter country name'));
			// $form_state['values']['country'] = $form_state['values']['other_country'];
		} //$form_state['values']['other_country'] == ''
		else
		{
			$form_state['values']['country'] = $form_state['values']['other_country'];
		}
		if ($form_state['values']['other_state'] == '')
		{
			form_set_error('other_state', t('Enter state name'));
			// $form_state['values']['country'] = $form_state['values']['other_country'];
		} //$form_state['values']['other_state'] == ''
		else
		{
			$form_state['values']['all_state'] = $form_state['values']['other_state'];
		}
		if ($form_state['values']['other_city'] == '')
		{
			form_set_error('other_city', t('Enter city name'));
			// $form_state['values']['country'] = $form_state['values']['other_country'];
		} //$form_state['values']['other_city'] == ''
		else
		{
			$form_state['values']['city'] = $form_state['values']['other_city'];
		}
	} //$form_state['values']['country'] == 'Others'
	else
	{
		if ($form_state['values']['country'] == '')
		{
			form_set_error('country', t('Select country name'));
			// $form_state['values']['country'] = $form_state['values']['other_country'];
		} //$form_state['values']['country'] == ''
		if ($form_state['values']['all_state'] == '')
		{
			form_set_error('all_state', t('Select state name'));
			// $form_state['values']['country'] = $form_state['values']['other_country'];
		} //$form_state['values']['all_state'] == ''
		if ($form_state['values']['city'] == '')
		{
			form_set_error('city', t('Select city name'));
			// $form_state['values']['country'] = $form_state['values']['other_country'];
		} //$form_state['values']['city'] == ''
	}
	//Validation for project title
	$form_state['values']['project_title'] = trim($form_state['values']['project_title']);
	if ($form_state['values']['project_title'] != '')
	{
		if (strlen($form_state['values']['project_title']) > 250)
		{
			form_set_error('project_title', t('Maximum charater limit is 250 charaters only, please check the length of the project title'));
		} //strlen($form_state['values']['project_title']) > 250
		else if (strlen($form_state['values']['project_title']) < 10)
		{
			form_set_error('project_title', t('Minimum charater limit is 10 charaters, please check the length of the project title'));
		} //strlen($form_state['values']['project_title']) < 10
	} //$form_state['values']['project_title'] != ''
	else
	{
		form_set_error('project_title', t('Project title shoud not be empty'));
	}
	$form_state['values']['description'] = trim($form_state['values']['description']);
	if ($form_state['values']['description'] != '')
	{
		if (strlen($form_state['values']['description']) > 700)
		{
			form_set_error('description', t('Maximum charater limit is 700 charaters only, please check the length of the description'));
		} //strlen($form_state['values']['project_title']) > 250
		else if (strlen($form_state['values']['description']) < 500)
		{
			form_set_error('description', t('Minimum charater limit is 500 charaters, please check the length of the description'));
		} //strlen($form_state['values']['project_title']) < 10
	} //$form_state['values']['project_title'] != ''
	else
	{
		form_set_error('description', t('Description shoud not be empty'));
	}
if (isset($_FILES['files']))
	{
		/* check if atleast one source or result file is uploaded */
		if (!($_FILES['files']['name']['samplefilepath']))
			form_set_error('samplefilepath', t('Please upload file with circuit diagram.'));
		/* check for valid filename extensions */
		foreach ($_FILES['files']['name'] as $file_form_name => $file_name)
		{
			if ($file_name)
			{
				/* checking file type */
				/*if (strstr($file_form_name, 'sample'))
					$file_type = 'S';
				else
					$file_type = 'U';
				
				/*switch ($file_type)
				{
					case 'S':
						$allowed_extensions_str = variable_get('textbook_companion_source_extensions', '');
						break;
				} *///$file_type
				$allowed_extensions_str = variable_get('resource_upload_extensions', '');
				$allowed_extensions = explode(',', $allowed_extensions_str);
				$fnames = explode('.', strtolower($_FILES['files']['name'][$file_form_name]));
				$temp_extension = end($fnames);
				if (!in_array($temp_extension, $allowed_extensions))
					form_set_error($file_form_name, t('Only file with ' . $allowed_extensions_str . ' extensions can be uploaded.'));
				if ($_FILES['files']['size'][$file_form_name] <= 0)
					form_set_error($file_form_name, t('File size cannot be zero.'));
				/* check if valid file name */
				if (!textbook_companion_check_valid_filename($_FILES['files']['name'][$file_form_name]))
					form_set_error($file_form_name, t('Invalid file name specified. Only alphabets and numbers are allowed as a valid filename.'));
			} //$file_name
		} //$_FILES['files']['name'] as $file_form_name => $file_name
	} 
	return $form_state;
}
function circuit_simulation_proposal_form_submit($form, &$form_state)
{
	global $user;
	$root_path = esim_circuit_simulation_path();
	if (!$user->uid)
	{
		drupal_set_message('It is mandatory to login on this website to access the proposal form', 'error');
		return;
	} //!$user->uid
	/*if ($form_state['values']['version'] == 'Old version')
	{
		$form_state['values']['version'] = trim($form_state['values']['older']);
	} *///$form_state['values']['version'] == 'Old version'
	/* inserting the user proposal */
	$v = $form_state["values"];
	$project_title = trim($v['project_title']);
	$proposar_name = $v['name_title'] . ' ' . $v['contributor_name'];
	$university = $v['university'];
	$directory_name = _df_dir_name($project_title, $proposar_name);
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
	$args = array(
		":uid" => $user->uid,
		":approver_uid" => 0,
		":name_title" => $v['name_title'],
		":contributor_name" => _df_sentence_case(trim($v['contributor_name'])),
		":contact_no" => $v['contributor_contact_no'],
		":university" => _df_sentence_case($v['university']),
		":city" => $v['city'],
		":pincode" => $v['pincode'],
		":state" => $v['all_state'],
		":country" => $v['country'],
		":project_guide_name" => _df_sentence_case($v['project_guide_name']),
		":project_guide_email_id" => trim($v['project_guide_email_id']),
		":project_title" => _df_sentence_case($v['project_title']),
		":description" => _df_sentence_case($v['description']),
		":operating_system" =>$v['operating_system'],
		":directory_name" => $directory_name,
		":approval_status" => 0,
		":is_completed" => 0,
		":dissapproval_reason" => "NULL",
		":creation_date" => time(),
		":approval_date" => 0,
		":samplefilepath" => "",
		":reference" => $v['reference']
		);
	//	var_dump($args);die;
	//var_dump($result);die;
	$result1 = db_query($result, $args, array(
		'return' => Database::RETURN_INSERT_ID
	));
//var_dump($args);die;

	$dest_path = $directory_name . '/';
	$dest_path1 = $root_path . $dest_path;
	//var_dump($dest_path1);die;	
	if (!is_dir($root_path . $dest_path))
		mkdir($root_path . $dest_path);
	/* uploading files */
	foreach ($_FILES['files']['name'] as $file_form_name => $file_name)
	{
		if ($file_name)
		{
			/* checking file type */
			//$file_type = 'S';
			if (file_exists($root_path . $dest_path . $_FILES['files']['name'][$file_form_name]))
			{
				 drupal_set_message(t("Error uploading file. File !filename already exists.", array('!filename' => $_FILES['files']['name'][$file_form_name])), 'error');
				//unlink($root_path . $dest_path . $_FILES['files']['name'][$file_form_name]);
			} //file_exists($root_path . $dest_path . $_FILES['files']['name'][$file_form_name])
			/* uploading file */
			if (move_uploaded_file($_FILES['files']['tmp_name'][$file_form_name], $root_path . $dest_path . $_FILES['files']['name'][$file_form_name]))
			{
				$query = "UPDATE {esim_circuit_simulation_proposal} SET samplefilepath = :samplefilepath WHERE id = :id";
				$args = array(
					":samplefilepath" => $dest_path . $_FILES['files']['name'][$file_form_name],
					":id" => $result1
				);
				
				$updateresult = db_query($query, $args);
				//var_dump($args);die;

				drupal_set_message($file_name . ' uploaded successfully.', 'status');
			} //move_uploaded_file($_FILES['files']['tmp_name'][$file_form_name], $root_path . $dest_path . $_FILES['files']['name'][$file_form_name])
			else
			{
				drupal_set_message('Error uploading file : ' . $dest_path . '/' . $file_name, 'error');
			}
		} //$file_name
	} //$_FILES['files']['name'] as $file_form_name => $file_name
	if (!$result1)
	{
		drupal_set_message(t('Error receiving your proposal. Please try again.'), 'error');
		return;
	} //!$proposal_id
	/* sending email */
	$email_to = $user->mail;
	$form = variable_get('circuit_simulation_from_email', '');
	$bcc = variable_get('circuit_simulation_emails', '');
	$cc = variable_get('circuit_simulation_cc_emails', '');
	$params['circuit_simulation_proposal_received']['result1'] = $result1;
	$params['circuit_simulation_proposal_received']['user_id'] = $user->uid;
	$params['circuit_simulation_proposal_received']['headers'] = array(
		'From' => $form,
		'MIME-Version' => '1.0',
		'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
		'Content-Transfer-Encoding' => '8Bit',
		'X-Mailer' => 'Drupal',
		'Cc' => $cc,
		'Bcc' => $bcc
	);
	if (!drupal_mail('circuit_simulation', 'circuit_simulation_proposal_received', $email_to, user_preferred_language($user), $params, $form, TRUE))
		drupal_set_message('Error sending email message.', 'error');
	drupal_set_message(t('We have received your eSim circuit simulation proposal. We will get back to you soon.'), 'status');
	drupal_goto('');
}

function esim_circuit_simulation_path() {
  return $_SERVER['DOCUMENT_ROOT'] . base_path() . 'esim_uploads/circuit_simulation_uploads/';
}