<?php
// $Id$
function circuit_simulation_download_upload_file()
{
	$proposal_id = arg(3);
	$root_path = circuit_simulation_document_path();
	$query = db_select('esim_circuit_simulation_proposal');
	$query->fields('esim_circuit_simulation_proposal');
	$query->condition('id', $proposal_id);
	$query->range(0, 1);
	$result = $query->execute();
	$circuit_simulation_upload_file = $result->fetchObject();
	$samplecodename = substr($circuit_simulation_upload_file->samplefilepath, strrpos($circuit_simulation_upload_file->samplefilepath, '/') + 1);
	//var_dump($samplecodename);die;
	//var_dump($root_path . $circuit_simulation_upload_file->samplefilepath);die;
	ob_clean();
            header("Pragma: public");
            header("Expires: 0");
            header("Cache-Control: must-revalidate, post-check=0, pre-check=0");
            header("Cache-Control: public");
            header("Content-Description: File Transfer");
            header('Content-Type: application/pdf');
            header('Content-disposition: attachment; filename="' . $samplecodename . '"');
            header('Content-Length: ' . filesize($root_path . $circuit_simulation_upload_file->samplefilepath));
            //var_dump($root_path . $circuit_simulation_upload_file->samplefilepath);die;
           // var_dump(filesize($root_path . $circuit_simulation_upload_file->directory_name . $circuit_simulation_upload_file->samplefilepath));
            header("Content-Transfer-Encoding: binary");
            header('Expires: 0');
            header('Pragma: no-cache');
            readfile($root_path . $circuit_simulation_upload_file->samplefilepath);
	       // var_dump(readfile($root_path . $circuit_simulation_upload_file->directory_name . $circuit_simulation_upload_file->samplefilepath));
	        ob_end_flush();
           ob_clean();
           flush();
	/*
	header('Content-Type: pdf');
	header('Content-disposition: attachment; filename="' . $samplecodename . '"');
	header('Content-Length: ' . filesize($root_path . $circuit_simulation_upload_file->directory_name . '/' . $circuit_simulation_upload_file->samplefilepath));

	ob_clean();
	readfile($root_path . $circuit_simulation_upload_file->directory_name . '/' . $circuit_simulation_upload_file->samplefilepath);
	//var_dump(readfile($root_path . $circuit_simulation_upload_file->directory_name . '/' . $circuit_simulation_upload_file->samplefilepath));die;*/
}
function circuit_simulation_download_solution_file()
{
	$solution_file_id = arg(3);
	$root_path = circuit_simulation_path();
	// $solution_files_q = db_query("SELECT * FROM {circuit_simulation_solution_files} WHERE id = %d LIMIT 1", $solution_file_id);
	$solution_files_q = db_query("SELECT lmsf.*, lmp.directory_name FROM esim_circuit_simulation_solution_files lmsf JOIN esim_circuit_simulation_solution lms JOIN esim_circuit_simulation_experiment lme JOIN esim_circuit_simulation_proposal lmp WHERE lms.id = lmsf.solution_id AND lme.id = lms.experiment_id AND lmp.id = lme.proposal_id AND lmsf.id = :solution_id LIMIT 1", array(
		':solution_id' => $solution_file_id
	));
	/*$query = db_select(esim_'circuit_simulation_solution_files');
	$query->fields('esim_circuit_simulation_solution_files');
	$query->condition('id', $solution_file_id);
	$query->range(0, 1);
	$solution_files_q = $query->execute();*/
	$solution_file_data = $solution_files_q->fetchObject();
	header('Content-Type: ' . $solution_file_data->filemime);
	//header('Content-Type: application/octet-stram');
	header('Content-disposition: attachment; filename="' . str_replace(' ', '_', ($solution_file_data->filename)) . '"');
	header('Content-Length: ' . filesize($root_path . $solution_file_data->directory_name . '/' . $solution_file_data->filepath));
	readfile($root_path . $solution_file_data->directory_name . '/' . $solution_file_data->filepath);
}
function circuit_simulation_download_dependency_file()
{
	$dependency_file_id = arg(3);
	$root_path = circuit_simulation_path();
	//$dependency_file_q = db_query("SELECT * FROM {circuit_simulation_dependency_files} WHERE id = %d LIMIT 1", $dependency_file_id);
	$query = db_select('esim_circuit_simulation_dependency_files');
	$query->fields('esim_circuit_simulation_dependency_files');
	$query->condition('id', $dependency_file_id);
	$query->range(0, 1);
	$dependency_files_q = $query->execute();
	$dependency_file_data = $dependency_files_q->fetchObject();
	header('Content-Type: ' . $dependency_file_data->filemime);
	header('Content-disposition: attachment; filename="' . str_replace(' ', '_', ($dependency_file_data->filename)) . '"');
	header('Content-Length: ' . filesize($root_path . $dependency_file_data->filepath));
	readfile($root_path . $dependency_file_data->filepath);
}
function circuit_simulation_download_solution()
{
	$solution_id = arg(3);
	$root_path = circuit_simulation_path();
	/* get solution data */
	//$solution_q = db_query("SELECT * FROM {esim_circuit_simulation_solution} WHERE id = %d", $solution_id);
	$query = db_select('esim_circuit_simulation_solution');
	$query->fields('esim_circuit_simulation_solution');
	$query->condition('id', $solution_id);
	$solution_q = $query->execute();
	$solution_data = $solution_q->fetchObject();
	//$experiment_q = db_query("SELECT * FROM {esim_circuit_simulation_experiment} WHERE id = %d", $solution_data->experiment_id);
	$query = db_select('esim_circuit_simulation_experiment');
	$query->fields('esim_circuit_simulation_experiment');
	$query->condition('id', $solution_data->experiment_id);
	$experiment_q = $query->execute();
	$experiment_data = $experiment_q->fetchObject();
	//$solution_files_q = db_query("SELECT * FROM {esim_circuit_simulation_solution_files} WHERE solution_id = %d", $solution_id);
	/*$query = db_select('esim_esim_circuit_simulation_solution_files');
	$query->fields('esim_circuit_simulation_solution_files');
	$query->condition('solution_id', $solution_id);
	$solution_files_q = $query->execute();*/
	$solution_files_q = db_query("SELECT lmsf.*, lmp.directory_name FROM esim_circuit_simulation_solution_files lmsf JOIN esim_circuit_simulation_solution lms JOIN esim_circuit_simulation_experiment lme JOIN esim_circuit_simulation_proposal lmp WHERE lms.id = lmsf.solution_id AND lme.id = lms.experiment_id AND lmp.id = lme.proposal_id AND lmsf.id = :solution_id", array(
		':solution_id' => $solution_id
	));
	//$solution_dependency_files_q = db_query("SELECT * FROM {circuit_simulation_solution_dependency} WHERE solution_id = %d", $solution_id);
	$query = db_select('esim_esim_circuit_simulation_solution_dependency');
	$query->fields('esim_circuit_simulation_solution_dependency');
	$query->condition('solution_id', $solution_id);
	$solution_dependency_files_q = $query->execute();
	$CODE_PATH = 'CODE' . $solution_data->code_number . '/';
	/* zip filename */
	$zip_filename = $root_path . 'zip-' . time() . '-' . rand(0, 999999) . '.zip';
	/* creating zip archive on the server */
	$zip = new ZipArchive;
	$zip->open($zip_filename, ZipArchive::CREATE);
	while ($solution_files_row = $solution_files_q->fetchObject())
	{
		$zip->addFile($root_path . $solution_files_row->directory_name . '/' . $solution_files_row->filepath, $CODE_PATH . str_replace(' ', '_', ($solution_files_row->filename)));
	} //$solution_files_row = $solution_files_q->fetchObject()
	/* dependency files */
	while ($solution_dependency_files_row = $solution_dependency_files_q->fetchObject())
	{
		//$dependency_file_data = (db_query("SELECT * FROM {circuit_simulation_dependency_files} WHERE id = %d LIMIT 1", $solution_dependency_files_row->dependency_id))->fetchObject();
		$query = db_select('esim_esim_circuit_simulation_dependency_files');
		$query->fields('esim_circuit_simulation_dependency_files');
		$query->condition('id', $solution_dependency_files_row->dependency_id);
		$query->range(0, 1);
		$dependency_file_data = $query->execute()->fetchObject();
		if ($dependency_file_data)
			$zip->addFile($root_path . $dependency_file_data->filepath, $CODE_PATH . 'DEPENDENCIES/' . str_replace(' ', '_', ($dependency_file_data->filename)));
	} //$solution_dependency_files_row = $solution_dependency_files_q->fetchObject()
	$zip_file_count = $zip->numFiles;
	$zip->close();
	if ($zip_file_count > 0)
	{
		/* download zip file */
		header('Content-Type: application/zip');
		header('Content-disposition: attachment; filename="CODE' . $solution_data->code_number . '.zip"');
		header('Content-Length: ' . filesize($zip_filename));
		ob_clean();
		//flush();
		readfile($zip_filename);
		unlink($zip_filename);
	} //$zip_file_count > 0
	else
	{
		drupal_set_message("There are no files in this solutions to download", 'error');
		drupal_goto('lab-migration/lab-migration-run');
	}
}
function circuit_simulation_download_experiment()
{
	$experiment_id = (int) arg(3);
	$root_path = circuit_simulation_path();
	/* get solution data */
	//$experiment_q = db_query("SELECT * FROM {esim_circuit_simulation_experiment} WHERE id = %d", $experiment_id);
	$query = db_select('esim_esim_circuit_simulation_experiment');
	$query->fields('esim_circuit_simulation_experiment');
	$query->condition('id', $experiment_id);
	$experiment_q = $query->execute();
	$experiment_data = $experiment_q->fetchObject();
	$EXP_PATH = 'EXP' . $experiment_data->number . '/';
	/* zip filename */
	$zip_filename = $root_path . 'zip-' . time() . '-' . rand(0, 999999) . '.zip';
	/* creating zip archive on the server */
	$zip = new ZipArchive;
	$zip->open($zip_filename, ZipArchive::CREATE);
	//$solution_q = db_query("SELECT * FROM {esim_circuit_simulation_solution} WHERE experiment_id = %d AND approval_status = 1", $experiment_id);
	$query = db_select('esim_esim_circuit_simulation_solution');
	$query->fields('esim_circuit_simulation_solution');
	$query->condition('experiment_id', $experiment_id);
	$query->condition('approval_status', 1);
	$solution_q = $query->execute();
	while ($solution_row = $solution_q->fetchObject())
	{
		$CODE_PATH = 'CODE' . $solution_row->code_number . '/';
		// $solution_files_q = db_query("SELECT * FROM {circuit_simulation_solution_files} WHERE solution_id = %d", $solution_row->id);
		$solution_files_q = db_query("SELECT lmsf.*, lmp.directory_name FROM esim_circuit_simulation_solution_files lmsf JOIN esim_circuit_simulation_solution lms JOIN esim_circuit_simulation_experiment lme JOIN esim_circuit_simulation_proposal lmp WHERE lms.id = lmsf.solution_id AND lme.id = lms.experiment_id AND lmp.id = lme.proposal_id AND lmsf.solution_id = :solution_id", array(
			':solution_id' => $solution_row->id
		));
		/* $query = db_select('esim_esim_circuit_simulation_solution_files');
		$query->fields('esim_circuit_simulation_solution_files');
		$query->condition('solution_id', $solution_row->id);
		$solution_files_q = $query->execute();*/
		// $solution_dependency_files_q = db_query("SELECT * FROM {circuit_simulation_solution_dependency} WHERE solution_id = %d", $solution_row->id);        
		while ($solution_files_row = $solution_files_q->fetchObject())
		{
			$zip->addFile($root_path . $solution_files_row->directory_name . '/' . $solution_files_row->filepath, $EXP_PATH . $CODE_PATH . str_replace(' ', '_', ($solution_files_row->filename)));
		} //$solution_files_row = $solution_files_q->fetchObject()
		/* dependency files */
		$query = db_select('esim_esim_circuit_simulation_solution_dependency');
		$query->fields('esim_circuit_simulation_solution_dependency');
		$query->condition('solution_id', $solution_row->id);
		$solution_dependency_files_q = $query->execute();
		while ($solution_dependency_files_row = $solution_dependency_files_q->fetchObject())
		{
			//$dependency_file_data = (db_query("SELECT * FROM {circuit_simulation_dependency_files} WHERE id = %d LIMIT 1", $solution_dependency_files_row->dependency_id))->fetchObject();
			$query = db_select('esim_esim_circuit_simulation_dependency_files');
			$query->fields('esim_circuit_simulation_dependency_files');
			$query->condition('id', $solution_dependency_files_row->dependency_id);
			$query->range(0, 1);
			$dependency_file_data = $query->execute()->fetchObject();
			if ($dependency_file_data)
				$zip->addFile($root_path . $dependency_file_data->filepath, $EXP_PATH . $CODE_PATH . 'DEPENDENCIES/' . str_replace(' ', '_', ($dependency_file_data->filename)));
		} //$solution_dependency_files_row = $solution_dependency_files_q->fetchObject()
	} //$solution_row = $solution_q->fetchObject()
	$zip_file_count = $zip->numFiles;
	$zip->close();
	if ($zip_file_count > 0)
	{
		/* download zip file */
		header('Content-Type: application/zip');
		header('Content-disposition: attachment; filename="EXP' . $experiment_data->number . '.zip"');
		header('Content-Length: ' . filesize($zip_filename));
		ob_clean();
		//flush();
		readfile($zip_filename);
		unlink($zip_filename);
	} //$zip_file_count > 0
	else
	{
		drupal_set_message("There are no solutions in this experiment to download", 'error');
		drupal_goto('lab-migration/lab-migration-run');
	}
}
function circuit_simulation_download_lab()
{
	global $user;
	$lab_id = arg(3);
	$root_path = circuit_simulation_path();
	/* get solution data */
	//$lab_q = db_query("SELECT * FROM {circuit_simulation_proposal} WHERE id = %d", $lab_id);
	$query = db_select('esim_circuit_simulation_proposal');
	$query->fields('esim_circuit_simulation_proposal');
	$query->condition('id', $lab_id);
	$lab_q = $query->execute();
	$lab_data = $lab_q->fetchObject();
	$LAB_PATH = $lab_data->lab_title . '/';
	/* zip filename */
	$zip_filename = $root_path . 'zip-' . time() . '-' . rand(0, 999999) . '.zip';
	/* creating zip archive on the server */
	$zip = new ZipArchive();
	$zip->open($zip_filename, ZipArchive::CREATE);
	//$experiment_q = db_query("SELECT * FROM {circuit_simulation_experiment} WHERE proposal_id = %d", $lab_id);
	$query = db_select('esim_circuit_simulation_experiment');
	$query->fields('esim_circuit_simulation_experiment');
	$query->condition('proposal_id', $lab_id);
	$experiment_q = $query->execute();
	while ($experiment_row = $experiment_q->fetchObject())
	{
		$EXP_PATH = 'EXP' . $experiment_row->number . '/';
		//$solution_q = db_query("SELECT * FROM {circuit_simulation_solution} WHERE experiment_id = %d AND approval_status = 1", $experiment_row->id);
		$query = db_select('esim_circuit_simulation_solution');
		$query->fields('esim_circuit_simulation_solution');
		$query->condition('experiment_id', $experiment_row->id);
		$query->condition('approval_status', 1);
		$solution_q = $query->execute();
		while ($solution_row = $solution_q->fetchObject())
		{
			$CODE_PATH = 'CODE' . $solution_row->code_number . '/';
			//$solution_files_q = db_query("SELECT * FROM {circuit_simulation_solution_files} WHERE solution_id = %d", $solution_row->id);
			$solution_files_q = db_query("SELECT lmsf.*, lmp.directory_name FROM esim_circuit_simulation_solution_files lmsf JOIN esim_circuit_simulation_solution lms JOIN esim_circuit_simulation_experiment lme JOIN esim_circuit_simulation_proposal lmp WHERE lms.id = lmsf.solution_id AND lme.id = lms.experiment_id AND lmp.id = lme.proposal_id AND lmsf.id = :solution_id", array(
				':solution_id' => $solution_row->id
			));
			/*$query = db_select('esim_esim_circuit_simulation_solution_files');
			$query->fields('esim_circuit_simulation_solution_files');
			$query->condition('solution_id', $solution_row->id);
			$solution_files_q = $query->execute();*/
			//$solution_dependency_files_q = db_query("SELECT * FROM {circuit_simulation_solution_dependency} WHERE solution_id = %d", $solution_row->id);
			$query = db_select('esim_circuit_simulation_solution_dependency');
			$query->fields('esim_circuit_simulation_solution_dependency');
			$query->condition('solution_id', $solution_row->id);
			$solution_dependency_files_q = $query->execute();
			while ($solution_files_row = $solution_files_q->fetchObject())
			{
				$zip->addFile($root_path . $solution_files_row->directory_name . '/' . $solution_files_row->filepath, $EXP_PATH . $CODE_PATH . str_replace(' ', '_', ($solution_files_row->filename)));
				//var_dump($zip->numFiles);
			} //$solution_files_row = $solution_files_q->fetchObject()
			// die;
			/* dependency files */
			while ($solution_dependency_files_row = $solution_dependency_files_q->fetchObject())
			{
				//$dependency_file_data = (db_query("SELECT * FROM {circuit_simulation_dependency_files} WHERE id = %d LIMIT 1", $solution_dependency_files_row->dependency_id))->fetchObject();
				$query = db_select('esim_circuit_simulation_dependency_files');
				$query->fields('esim_circuit_simulation_dependency_files');
				$query->condition('id', $solution_dependency_files_row->dependency_id);
				$query->range(0, 1);
				$dependency_file_data = $query->execute()->fetchObject();
				if ($dependency_file_data)
					$zip->addFile($root_path . $dependency_file_data->filepath, $EXP_PATH . $CODE_PATH . 'DEPENDENCIES/' . str_replace(' ', '_', ($dependency_file_data->filename)));
			} //$solution_dependency_files_row = $solution_dependency_files_q->fetchObject()
		} //$solution_row = $solution_q->fetchObject()
	} //$experiment_row = $experiment_q->fetchObject()
	$zip_file_count = $zip->numFiles;
	$zip->close();
	if ($zip_file_count > 0)
	{
		if ($user->uid)
		{
			/* download zip file */
			header('Content-Type: application/zip');
			header('Content-disposition: attachment; filename="' . str_replace(' ', '_', $lab_data->lab_title) . '.zip"');
			header('Content-Length: ' . filesize($zip_filename));
			ob_clean();
			//flush();
			readfile($zip_filename);
			unlink($zip_filename);
		} //$user->uid
		else
		{
			header('Content-Type: application/zip');
			header('Content-disposition: attachment; filename="' . str_replace(' ', '_', $lab_data->lab_title) . '.zip"');
			header('Content-Length: ' . filesize($zip_filename));
			header("Content-Transfer-Encoding: binary");
			header('Expires: 0');
			header('Pragma: no-cache');
			ob_end_flush();
			ob_clean();
			flush();
			readfile($zip_filename);
			unlink($zip_filename);
		}
	} //$zip_file_count > 0
	else
	{
		drupal_set_message("There are no solutions in this Lab to download", 'error');
		drupal_goto('lab-migration/lab-migration-run');
	}
}
