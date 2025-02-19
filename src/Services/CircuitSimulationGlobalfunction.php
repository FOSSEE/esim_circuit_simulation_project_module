<?php
 
namespace Drupal\circuit_simulation\Services;

use Drupal\Core\Extension\ModuleHandlerInterface;
use Drupal\Core\File\FileSystemInterface;
use Drupal\Core\Link;
use Drupal\Core\Url;
use Symfony\Component\HttpFoundation\RedirectResponse;
use Drupal\Core\Database\Database;
use Drupal\Core\DrupalKernel;
use Drupal\user\Entity\User;
use Drupal\Core\Routing\TrustedRedirectResponse;
use Symfony\Component\DependencyInjection\ContainerInterface;
use Drupal\Core\Messenger\MessengerInterface;


class CircuitSimulationGlobalfunction{
  function _esim_cs_list_of_states()
{
  $states = array(
    0 => '-Select-'
  );
  $query = \Drupal::database()->select('list_states_of_india');
  $query->fields('list_states_of_india');
  //$query->orderBy('', '');
  $states_list = $query->execute();
  while ($states_list_data = $states_list->fetchObject())
  {
    $states[$states_list_data->state] = $states_list_data->state;
  } //$states_list_data = $states_list->fetchObject()
  return $states;
}
function _esim_cs_list_of_cities()
{
  $city = array(
    0 => '-Select-'
  );
  $query = \Drupal::database()->select('list_cities_of_india');
  $query->fields('list_cities_of_india');
  $query->orderBy('city', 'ASC');
  $city_list = $query->execute();
  while ($city_list_data = $city_list->fetchObject())
  {
    $city[$city_list_data->city] = $city_list_data->city;
  } //$city_list_data = $city_list->fetchObject()
  return $city;
}
function _esim_cs_list_of_pincodes()
{
  $pincode = array(
    0 => '-Select-'
  );
  $query = \Drupal::database()->select('list_of_all_india_pincode');
  $query->fields('list_of_all_india_pincode');
  $query->orderBy('pincode', 'ASC');
  $pincode_list = $query->execute();
  while ($pincode_list_data = $pincode_list->fetchObject())
  {
    $pincode[$pincode_list_data->pincode] = $pincode_list_data->pincode;
  } //$pincode_list_data = $pincode_list->fetchObject()
  return $pincode;
}
function _esim_cs_list_of_departments()
{
  $department = array();
  $query = \Drupal::database()->select('list_of_departments');
  $query->fields('list_of_departments');
  $query->orderBy('id', 'DESC');
  $department_list = $query->execute();
  while ($department_list_data = $department_list->fetchObject())
  {
    $department[$department_list_data->department] = $department_list_data->department;
  } //$department_list_data = $department_list->fetchObject()
  return $department;
}

   function default_value_for_uploaded_files($filetype, $proposal_id) {
    $selected_files_array = null;

    if (in_array($filetype, ['A', 'S'])) {
        $query = Database::getConnection()->select('esim_circuit_simulation_submitted_abstracts_file', 'f');
        $query->fields('f');
        $query->condition('proposal_id', $proposal_id);
        $query->condition('filetype', $filetype);
        $selected_files_array = $query->execute()->fetchObject();
    }

    return $selected_files_array;
}
function circuit_simulation_get_proposal() {
  // Get the current user service.
  $current_user = \Drupal::currentUser();
  $uid = $current_user->id();

  // Fetch the latest proposal for the current user.
  $query = Database::getConnection()->select('esim_circuit_simulation_proposal', 'p')
    ->fields('p')
    ->condition('uid', $uid)
    ->orderBy('id', 'DESC')
    ->range(0, 1);
  $proposal_data = $query->execute()->fetchObject();

  // Handle cases when no proposal is found.
  if (!$proposal_data) {
    \Drupal::messenger()->addError(t('You do not have any approved eSim Circuit Simulation proposal. Please propose the circuit simulation proposal.'));
    return new TrustedRedirectResponse('/');
  }

  // Check the approval status of the proposal.
  switch ($proposal_data->approval_status) {
    case 0:
      \Drupal::messenger()->addStatus(t('Proposal is awaiting approval.'));
      return FALSE;

    case 1:
      return $proposal_data;

    case 2:
      \Drupal::messenger()->addError(t('Proposal has been disapproved.'));
      return FALSE;

    case 3:
      \Drupal::messenger()->addStatus(t('Proposal has been marked as completed.'));
      return FALSE;

    default:
      \Drupal::messenger()->addError(t('Invalid proposal state. Please contact the site administrator for further information.'));
      return FALSE;
  }
}
function _esim_cs_sentence_case($string)
{
  $string = ucwords(strtolower($string));
  foreach (array(
    '-',
    '\''
  ) as $delimiter)
  {
    if (strpos($string, $delimiter) !== false)
    {
      $string = implode($delimiter, array_map('ucfirst', explode($delimiter, $string)));
    } //strpos($string, $delimiter) !== false
  } //array( '-', '\'' ) as $delimiter
  return $string;
}
function circuit_simulation_check_valid_filename($file_name)
{
  if (!preg_match('/^[0-9a-zA-Z\.\_]+$/', $file_name))
    return FALSE;
  else if (substr_count($file_name, ".") > 1)
    return FALSE;
  else
    return TRUE;
}
function circuit_simulation_path()
{
  return $_SERVER['DOCUMENT_ROOT'] . base_path() . 'esim_uploads/circuit_simulation_uploads/';
}
function CreateReadmeFileeSimCircuitSimulationProject($proposal_id)
{
  $result = \Drupal::database()->query("
                        SELECT * from esim_circuit_simulation_proposal WHERE id = :proposal_id", array(
    ":proposal_id" => $proposal_id
  ));
  $proposal_data = $result->fetchObject();
  $root_path = circuit_simulation_path();
  $readme_file = fopen($root_path . $proposal_data->directory_name . "/README.txt", "w") or die("Unable to open file!");
  $txt = "";
  $txt .= "About the Circuit Simulation";
  $txt .= "\n" . "\n";
  $txt .= "Title Of The Circuit Simulation Project: " . $proposal_data->project_title . "\n";
  $txt .= "Proposar Name: " . $proposal_data->name_title . " " . $proposal_data->contributor_name . "\n";
  $txt .= "University: " . $proposal_data->university . "\n";
  $txt .= "\n" . "\n";
  $txt .= "eSim Circuit Simulation Project By FOSSEE, IIT Bombay" . "\n";
  fwrite($readme_file, $txt);
  fclose($readme_file);
  return $txt;
}
function rrmdir_project($prop_id)
{
  $proposal_id = $prop_id;
  $result = \Drupal::database()->query("
          SELECT * from esim_circuit_simulation_proposal WHERE id = :proposal_id", array(
    ":proposal_id" => $proposal_id
  ));
  $proposal_data = $result->fetchObject();
  $root_path = circuit_simulation_path();
  $dir = $root_path . $proposal_data->directory_name;
  if ($proposal_data->id == $prop_id)
  {
    if (is_dir($dir))
    {
      $objects = scandir($dir);
      foreach ($objects as $object)
      {
        if ($object != "." && $object != "..")
        {
          if (filetype($dir . "/" . $object) == "dir")
          {
            rrmdir($dir . "/" . $object);
          } //filetype($dir . "/" . $object) == "dir"
          else
          {
            unlink($dir . "/" . $object);
          }
        } //$object != "." && $object != ".."
      } //$objects as $object
      reset($objects);
      rmdir($dir);
      $msg = \Drupal::messenger()->addMessage("Directory deleted successfully");
      return $msg;
    } //is_dir($dir)
    $msg = \Drupal::messenger()->addMessage("Directory not present");
    return $msg;
  } //$proposal_data->id == $prop_id
  else
  {
    $msg = \Drupal::messenger()->addMessage("Data not found");
    return $msg;
  }
}
function rrmdir($dir)
{
  if (is_dir($dir))
  {
    $objects = scandir($dir);
    foreach ($objects as $object)
    {
      if ($object != "." && $object != "..")
      {
        if (filetype($dir . "/" . $object) == "dir")
          rrmdir($dir . "/" . $object);
        else
          unlink($dir . "/" . $object);
      } //$object != "." && $object != ".."
    } //$objects as $object
    reset($objects);
    rmdir($dir);
  } //is_dir($dir)
}
function _cs_dir_name($project, $proposar_name)
{
  $project_title = preg_replace("/_+/", "_",preg_replace('/[^A-Za-z0-9\-]/', '_', $project));
  $proposar_name = $proposar_name;
  $dir_name = $project_title . ' By ' . $proposar_name;
  $directory_name = str_replace("__", "_", str_replace(" ", "_", str_replace("/","_", trim($dir_name))));
  //var_dump($directory_name);die;
  return $directory_name;
}
function CS_RenameDir($proposal_id, $dir_name)
{
  $proposal_id = $proposal_id;
  $dir_name = $dir_name;
  $root_path = circuit_simulation_path();
  $query = \Drupal::database()->query("SELECT directory_name,id FROM esim_circuit_simulation_proposal WHERE id = :proposal_id", array(
    ':proposal_id' => $proposal_id
  ));
  $result = $query->fetchObject();
  if ($result != NULL)
  {
    $files = scandir($root_path);
    $files_id_dir = $root_path . $result->id;
    //var_dump($files);die;
    $file_dir = $root_path . $result->directory_name;
    if (is_dir($file_dir))
    {
      $new_directory_name = rename($root_path . $result->directory_name, $root_path . $dir_name);
      return $new_directory_name;
    } //is_dir($file_dir)
    else if (is_dir($files_id_dir))
    {
      $new_directory_name = rename($root_path . $result->id, $root_path . $dir_name);
      return $new_directory_name;
    } //is_dir($files_id_dir)
    else
    {
      \Drupal::messenger()->addMessage('Directory not available for rename.');
      return;
    }
  } //$result != NULL
  else
  {
    \Drupal::messenger()->addMessage('Project directory name not present in databse');
    return;
  }
  return;
}
function circuit_simulation_abstract_delete_project($proposal_id) {
  $status = TRUE;
  $root_path = circuit_simulation_path();

  // Fetch the proposal data.
  $connection = \Drupal::database();
  $query = $connection->select('esim_circuit_simulation_proposal', 'e')
    ->fields('e')
    ->condition('id', $proposal_id);
  $proposal_data = $query->execute()->fetchObject();

  if (!$proposal_data) {
    \Drupal::messenger()->addError('Invalid circuit simulation project.');
    return FALSE;
  }

  // Fetch associated abstract files.
  $abstract_query = $connection->select('esim_circuit_simulation_submitted_abstracts_file', 'a')
    ->fields('a')
    ->condition('proposal_id', $proposal_id);
  $abstract_results = $abstract_query->execute();

  // Delete abstract files.
  $dir_project_files = $root_path . $proposal_data->directory_name . '/project_files';
  foreach ($abstract_results as $abstract_data) {
    if (is_dir($dir_project_files)) {
      $file_path = $root_path . $proposal_data->directory_name . '/project_files/' . $abstract_data->filepath;
      if (file_exists($file_path)) {
        unlink($file_path);
      }
    } else {
      \Drupal::messenger()->addError('Invalid circuit simulation project abstract.');
    }
  }

  // Remove project files directory.
  if (is_dir($dir_project_files)) {
    rmdir($dir_project_files);
  }

  // Remove root directory.
  $dir_path_udc = $root_path . $proposal_data->directory_name;
  if (is_dir($dir_path_udc)) {
    $sample_file_path = $root_path . $proposal_data->samplefilepath;
    if (file_exists($sample_file_path)) {
      unlink($sample_file_path);
    }
    rmdir($dir_path_udc);
  }

  // Delete records from the database.
  $connection->delete('esim_circuit_simulation_proposal')
    ->condition('id', $proposal_data->id)
    ->execute();

  $connection->delete('esim_circuit_simulation_submitted_abstracts')
    ->condition('proposal_id', $proposal_id)
    ->execute();

  $connection->delete('esim_circuit_simulation_submitted_abstracts_file')
    ->condition('proposal_id', $proposal_id)
    ->execute();

  return $status;
}
}
 